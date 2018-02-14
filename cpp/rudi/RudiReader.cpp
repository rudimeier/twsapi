#include "StdAfx.h"
#include "EDecoder.h"
#include "EWrapper.h"
#include "TwsSocketClientErrors.h"
#include "RudiReader.h"
#include "RudiClient.h"
#include "EPosixClientSocketPlatform.h"

#include <assert.h>
#include <string.h>

#define IN_BLOCK_SIZE (8 * 1024)

struct read_buf {
	char *begin;
	size_t size;
	size_t offset; /* already consumed */
};

static void init_read_buf(struct read_buf *p)
{
	memset(p, 0, sizeof(*p));
}

static void deinit_read_buf(struct read_buf *p)
{
	free(p->begin);
}

static int resize_read_buf(struct read_buf *p, size_t s)
{
	char *buf;
	buf = (char*)realloc(p->begin, s);
	if (buf) {
		p->begin = buf;
		p->size = s;
		return 0;
	}
	/* original buf not changed */
	return -1;
}

static uint32_t read_uint32(const char *p)
{
	uint32_t val;
	memcpy(&val, p, sizeof(val));
	return ntohl(val);
}

RudiReader::RudiReader(RudiClient *clientSocket)
{
	m_pClientSocket = clientSocket;
	decoder = new EDecoder(clientSocket->EClient::serverVersion(),
		clientSocket->getWrapper(), clientSocket);
	m_buf = new read_buf();
	init_read_buf(m_buf);
	int err = resize_read_buf(m_buf, IN_BLOCK_SIZE);
	assert(!err); /* TODO error handling ENOMEM */
}

RudiReader::~RudiReader(void)
{
	deinit_read_buf(m_buf);
	delete m_buf;
	delete decoder;
}


void RudiReader::select_timeout( int msec )
{
	assert( msec >= 0 );

	struct timeval tval;
	tval.tv_sec = msec / 1000 ;
	tval.tv_usec = (msec % 1000) * 1000;

	fd_set readSet, writeSet;

	FD_ZERO( &readSet);
	FD_ZERO( &writeSet);

	int fd = -1;
	if( m_pClientSocket->isConnected() ) {
		// if not connected then all sets are zero and select will just timeout
		fd = m_pClientSocket->fd();
		assert( fd >= 0 );

		FD_SET( fd, &readSet);
		if( !m_pClientSocket->getTransport()->isOutBufferEmpty()) {
			FD_SET( fd, &writeSet);
		}
	}
	int ret = select( fd + 1,
		&readSet, &writeSet, NULL, &tval );
	/////  blocking  ///////////////////////////////////////

	if( ret == 0) {
		TWS_DEBUG( 5 , "Select timeouted." );
		return;
	} else if( ret < 0) {
		TWS_DEBUG( 1 , "Select failed: %s, fd: %d, timval: (%lds, %ldus).",
			strerror(errno), fd, tval.tv_sec, tval.tv_usec );
		m_pClientSocket->onClose();
		return;
	}

	if( FD_ISSET( fd, &writeSet)) {
		TWS_DEBUG( 1 ,"Socket is ready for writing." );
		m_pClientSocket->onSend(); // might disconnect us on socket errors
		if( m_pClientSocket->isConnected() ) {
			return;
		}
	}

	if( FD_ISSET( fd, &readSet)) {
		TWS_DEBUG( 6 ,"Socket is ready for reading." );
		onReceive(); // might disconnect us on socket errors
	}
}

#if 0
bool RudiReader::processNonBlockingSelect()
{
	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

	tval.tv_usec = 100 * 1000; //100 ms
	tval.tv_sec = 0;

	if( m_pClientSocket->fd() >= 0 ) {

		FD_ZERO( &readSet);
		errorSet = writeSet = readSet;

		FD_SET( m_pClientSocket->fd(), &readSet);

		if (!m_pClientSocket->getTransport()->isOutBufferEmpty())
			FD_SET( m_pClientSocket->fd(), &writeSet);

		FD_SET( m_pClientSocket->fd(), &errorSet);

		int ret = select( m_pClientSocket->fd() + 1, &readSet, &writeSet, &errorSet, &tval);

		if( ret == 0) { // timeout
			return false;
		}

		if( ret < 0) {	// error
			m_pClientSocket->eDisconnect();
			return false;
		}

		if( m_pClientSocket->fd() < 0)
			return false;

		if( FD_ISSET( m_pClientSocket->fd(), &errorSet)) {
			// error on socket
			m_pClientSocket->onError();
		}

		if( m_pClientSocket->fd() < 0)
			return false;

		if( FD_ISSET( m_pClientSocket->fd(), &writeSet)) {
			// socket is ready for writing
			m_pClientSocket->onSend();
		}

		if( m_pClientSocket->fd() < 0)
			return false;

		if( FD_ISSET( m_pClientSocket->fd(), &readSet)) {
			// socket is ready for reading
			onReceive();
		}

		return true;
	}

	return false;
}
#endif

void RudiReader::onReceive()
{
	const char * errmsg;
	size_t sz = m_buf->size - m_buf->offset;
	int nRes = m_pClientSocket->receive(m_buf->begin + m_buf->offset, sz );
	TWS_DEBUG(1, "received: %d (tried %zu)", nRes, sz);
	if (nRes <= 0) {
		errmsg = nRes < 0 ? strerror(errno)
			: "The remote host closed the connection.";
		goto fail;
	}

	m_buf->offset += nRes;

	assert(m_pClientSocket->usingV100Plus());
	if (readV100Plus() == -1) {
		errmsg = strerror(errno);
		goto fail;
	}
	return;

fail:
	m_pClientSocket->getWrapper()->error(
		NO_VALID_ID, SOCKET_EXCEPTION.code(), errmsg );
	m_pClientSocket->onClose();
	return;
}


/* decoder error handling */
int RudiReader::decode_one_msg(const char* msgbegin, uint32_t msgsize)
{
	const char *p = msgbegin;
	const char *msgend = msgbegin + msgsize;
	int processed;

	/* this increments p already! */
	processed = decoder->parseAndProcessMsg( p, msgend);

	assert( processed >= 0 );
	assert( processed == p - msgbegin );

	/* processed == 0 may happen for 3 reasons:
	 * 1. could not parse a whole message. That's always a protocol error
	 *    since we are using V100Plus.
	 * 2. caught exception while parsing. That's also a protocol error.
	 * 3. caught exception from the users's callback ... The decoder should not
	 *    do that. For now we handle that also as EPROTO but should be fixed.
	 */
	if (processed == 0) {
		errno = EPROTO;
		return -1;
	}

	/* protocol error, violationg V100Plus */
	if ((uint32_t)processed != msgsize) {
		errno = EPROTO;
		return -1;
	}
	return processed;
}

int RudiReader::readV100Plus()
{
	const char *end = m_buf->begin + m_buf->offset;
	const char *p = m_buf->begin;
	uint32_t msgSize;

	while (p < end) {
		int processed;

		if (end < p + sizeof(msgSize)) {
			TWS_DEBUG(1, "incomplete message size: %zu < %zu",
				end - p, sizeof(msgSize));
			msgSize = sizeof(msgSize);
			goto reshape;
		}
		msgSize = read_uint32(p);
		if (msgSize == 0 || msgSize > MAX_MSG_LEN) {
			errno = EPROTO;
			goto fail;
		}

		TWS_DEBUG(2, "got msgSize %zu", (size_t)msgSize);
		if (end < p + sizeof(msgSize) + msgSize) {
			TWS_DEBUG(1, "incomplete message %zu < %zu",
				end - p - sizeof(msgSize), (size_t)msgSize);
			goto reshape;
		}
		p += sizeof(msgSize);

		processed = decode_one_msg( p, msgSize);
		TWS_DEBUG(1, "processed %d", processed);
		if( processed == -1 ) {
			goto fail;
		}
		p += processed;
	}
	assert(p == end);
	m_buf->offset = 0;
	return 0;

reshape:
	if( p - m_buf->begin > 0) {
		size_t left = end - p;
		TWS_DEBUG(1, "memmove %zu", left);
		memmove(m_buf->begin, p, left);
		m_buf->offset = left;
	}

	/* let the last incomplete message fit into the buffer */
	if( msgSize > m_buf->size) {
		size_t newsize = IN_BLOCK_SIZE * (1 + msgSize/IN_BLOCK_SIZE);
		TWS_DEBUG(1, "resize: offset:  %zu oldsize: %zu newsize: %zu",
			m_buf->offset, m_buf->size, newsize);
		if (resize_read_buf(m_buf, newsize) == -1) {
			goto fail;
		}
	}
	return 0;

fail:
	TWS_DEBUG(0, "error readV100Plus: %s" , strerror(errno));
	return -1;
}


