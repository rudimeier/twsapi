#pragma once

class EPosixClientSocket;
class EDecoder;
struct read_buf;

class TWSAPIDLLEXP EReaderST
{
public:
	EReaderST(EPosixClientSocket *clientSocket);
	~EReaderST(void);

#if 0
	bool processNonBlockingSelect();
#endif
	void select_timeout( int msec );
	void onReceive();

private:
	int readV100Plus();
	int decode_one_msg(const char* begin, uint32_t size);

	EPosixClientSocket *m_pClientSocket;
	EDecoder *decoder;
	struct read_buf *m_buf;
};
