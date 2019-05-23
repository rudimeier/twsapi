/* Copyright (C) 2018 Ruediger Meier
 * Author:  Ruediger Meier <sweet_f_a@gmx.de>
 * License: BSD 3-Clause */

#include "StdAfx.h"
#include "EDecoder.h"
#include "EWrapper.h"
#include "TwsSocketClientErrors.h"
#include "EReaderST.h"
#include "EPosixClientSocket.h"
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

EReaderST::EReaderST(EPosixClientSocket *clientSocket)
{
	m_pClientSocket = clientSocket;
	decoder = new EDecoder(clientSocket->EClient::serverVersion(),
		clientSocket->getWrapper(), clientSocket);
	m_buf = new read_buf();
	init_read_buf(m_buf);
	int err = resize_read_buf(m_buf, IN_BLOCK_SIZE);
	assert(!err); /* TODO error handling ENOMEM */
}

EReaderST::~EReaderST(void)
{
	deinit_read_buf(m_buf);
	delete m_buf;
	delete decoder;
}

void EReaderST::reset()
{
	m_buf->offset = 0;
	/* there is no public method to reset EDecoder's m_serverVersion ... */
	delete decoder;
	decoder = new EDecoder(m_pClientSocket->EClient::serverVersion(),
		m_pClientSocket->getWrapper(), m_pClientSocket);
}

void EReaderST::select_timeout( int msec )
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
		return;
	} else if( ret < 0) {
		m_pClientSocket->onClose();
		return;
	}

	if( FD_ISSET( fd, &writeSet)) {
		m_pClientSocket->onSend(); // might disconnect us on socket errors
		if( m_pClientSocket->isConnected() ) {
			return;
		}
	}

	if( FD_ISSET( fd, &readSet)) {
		onReceive(); // might disconnect us on socket errors
	}
}

void EReaderST::onReceive()
{
	const char * errmsg;
	if (m_buf->offset == m_buf->size) {
		if (resize_read_buf(m_buf, m_buf->size + IN_BLOCK_SIZE) == -1) {
			errmsg = strerror(errno);
			goto fail;
		}
	}
	{
		size_t sz = m_buf->size - m_buf->offset;
		int nRes = m_pClientSocket->receive(m_buf->begin + m_buf->offset, sz );
		if (nRes <= 0) {
			errmsg = nRes < 0 ? strerror(errno)
				: "The remote host closed the connection.";
			goto fail;
		}

		m_buf->offset += nRes;
	}

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
int EReaderST::decode_one_msg(const char* msgbegin, uint32_t msgsize)
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

int EReaderST::readV100Plus()
{
	const char *end = m_buf->begin + m_buf->offset;
	const char *p = m_buf->begin;
	uint32_t msgSize;

	while (p < end) {
		int processed;

		if (end < p + sizeof(msgSize)) {
			msgSize = sizeof(msgSize);
			goto reshape;
		}
		msgSize = read_uint32(p);
		if (msgSize == 0 || msgSize > MAX_MSG_LEN) {
			errno = EPROTO;
			goto fail;
		}

		if (end < p + sizeof(msgSize) + msgSize) {
			goto reshape;
		}
		p += sizeof(msgSize);

		processed = decode_one_msg( p, msgSize);
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
		memmove(m_buf->begin, p, left);
		m_buf->offset = left;
	}

	/* let the last incomplete message fit into the buffer */
	if( msgSize > m_buf->size) {
		size_t newsize = IN_BLOCK_SIZE * ((msgSize + IN_BLOCK_SIZE - 1)/IN_BLOCK_SIZE);
		if (resize_read_buf(m_buf, newsize) == -1) {
			goto fail;
		}
	}
	return 0;

fail:
	return -1;
}


