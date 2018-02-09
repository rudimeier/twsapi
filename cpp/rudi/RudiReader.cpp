#include "StdAfx.h"
#include "EDecoder.h"
#include "RudiReader.h"
#include "EClientSocket.h"
#include "EPosixClientSocketPlatform.h"

#include <assert.h>
#include <string.h>

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

RudiReader::RudiReader(EClientSocket *clientSocket)
{
	m_pClientSocket = clientSocket;
	m_needsWriteSelect = false;
	decoder = new EDecoder(clientSocket->EClient::serverVersion(),
		clientSocket->getWrapper(), clientSocket);
	m_buf = new read_buf();
	init_read_buf(m_buf);
	resize_read_buf(m_buf, IN_BLOCK_SIZE);
}

RudiReader::~RudiReader(void)
{
	deinit_read_buf(m_buf);
	delete m_buf;
	delete decoder;
}

bool RudiReader::processNonBlockingSelect() {
	fd_set readSet, writeSet, errorSet;
	struct timeval tval;

	tval.tv_usec = 100 * 1000; //100 ms
	tval.tv_sec = 0;

	if( m_pClientSocket->fd() >= 0 ) {

		FD_ZERO( &readSet);
		errorSet = writeSet = readSet;

		FD_SET( m_pClientSocket->fd(), &readSet);

		if (m_needsWriteSelect)
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
			onSend();
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

void RudiReader::onSend() {
}

void RudiReader::onReceive()
{
	assert(m_buf->size - m_buf->offset >= IN_BLOCK_SIZE);

	int nRes = m_pClientSocket->receive(m_buf->begin + m_buf->offset, IN_BLOCK_SIZE);
	fprintf(stderr, "receive %s %d\n", thread_str(), nRes);
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
	size_t consumed;
	uint32_t msgSize;

	while (p < end) {
		uint32_t processed;

		if (p + sizeof(msgSize) > end) {
			fprintf(stderr, "not even message size: %lu < %lu\n",
				m_buf->offset, sizeof(msgSize));
			break;
		}

		msgSize = ntohl( *(uint32_t*)p);
		fprintf(stderr, "got msgSize %u\n", msgSize);
		if (p + sizeof(msgSize) + msgSize > end) {
			fprintf(stderr, "message still incomplete %zu < %u\n",
				m_buf->offset, msgSize);
			break;
		}
		p += sizeof(msgSize);

		/* this increments p already! */
		processed = decoder->parseAndProcessMsg( p, p + msgSize);
		fprintf(stderr, "processed %u %p\n", processed, p);
		assert( processed == msgSize );
	}
	consumed = p - m_buf->begin;
	fprintf(stderr, "buf consumed: %zu, size: %zu / %zu %s\n",
		consumed, m_buf->offset, m_buf->size,
		consumed != m_buf->offset ? "-> memmove" : "");
	assert( consumed <= m_buf->offset );
	if( consumed != m_buf->offset ) {
		memmove(m_buf->begin, p, consumed);
	}
	m_buf->offset -= consumed;

	/* let the last incomplete message fit into the buffer */
	if(msgSize > m_buf->size) {
		int err;
		size_t newsize = (msgSize+1) * (msgSize/IN_BLOCK_SIZE);
		err = resize_read_buf(m_buf, newsize);
		fprintf(stderr, "resize %zu  %zu %d\n", newsize,m_buf->size, err );
		assert(!err); // TODO error handling
	}
}


