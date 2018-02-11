#include "StdAfx.h"
#include "EDecoder.h"
#include "RudiReader.h"
#include "RudiClient.h"
#include "EPosixClientSocketPlatform.h"

#include <assert.h>
#include <string.h>

// debug verbosity 0...n
#define tws_debug_level 1
#define TWS_DEBUG( _level, _fmt, _msg... )        \
	if( tws_debug_level >= _level ) {    \
		fprintf (stderr, "TWS_DEBUG: " _fmt "\n" , ## _msg); \
	}

#define IN_BLOCK_SIZE 4096
/* TODO there should be a hard MAX_BUF_SIZE to protect against bad servers*/

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
		m_pClientSocket->eDisconnect();
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

void RudiReader::onReceive()
{
	int nRes = m_pClientSocket->receive(m_buf->begin + m_buf->offset,
		m_buf->size - m_buf->offset);
	TWS_DEBUG(2, "received %d", nRes);
	if (nRes <= 0) {
		assert( nRes > 0 );
		return; // TODO error handling
	}
	m_buf->offset += nRes;

	assert(m_pClientSocket->usingV100Plus());
	readV100Plus();
}

void RudiReader::readV100Plus()
{
	const char *end = m_buf->begin + m_buf->offset;
	const char *p = m_buf->begin;
	uint32_t msgSize;

	while (p < end) {
		uint32_t processed;

		if (end < p + sizeof(msgSize)) {
			TWS_DEBUG(1, "incomplete message size: %zu < %zu",
				end - p, sizeof(msgSize));
			msgSize = sizeof(msgSize);
			goto reshape;
		}
		msgSize = read_uint32(p);

		TWS_DEBUG(2, "got msgSize %zu", (size_t)msgSize);
		if (end < p + sizeof(msgSize) + msgSize) {
			TWS_DEBUG(1, "incomplete message %zu < %zu",
				end - p - sizeof(msgSize), (size_t)msgSize);
			goto reshape;
		}
		p += sizeof(msgSize);

		/* this increments p already! */
		processed = decoder->parseAndProcessMsg( p, p + msgSize);
		TWS_DEBUG(1, "processed %u", processed);
		assert( processed == msgSize );
	}
	assert(p == end);
	m_buf->offset = 0;
	return;

reshape:
	size_t consumed = p - m_buf->begin;

	if( consumed > 0) {
		size_t left = end - p;
		TWS_DEBUG(1, "memmove %zu", left);
		memmove(m_buf->begin, p, left);
		m_buf->offset = left;
	}

	/* let the last incomplete message fit into the buffer */
	if( msgSize > m_buf->size) {
		int err;
		size_t newsize = IN_BLOCK_SIZE * (1 + msgSize/IN_BLOCK_SIZE);
		err = resize_read_buf(m_buf, newsize);
		TWS_DEBUG(1, "resized: offset: %zu newsize: %zu, err: %d",
			m_buf->offset, newsize, err );
		assert(!err); /* TODO error handling ENOMEM */
	}
}


