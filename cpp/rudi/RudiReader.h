#pragma once

class EClientSocket;
class EDecoder;
struct read_buf;

class TWSAPIDLLEXP RudiReader
{
public:
	RudiReader(EClientSocket *clientSocket);
	~RudiReader(void);

	bool processNonBlockingSelect();
	void onSend();
	void onReceive();

private:
	void readV100Plus();

	EClientSocket *m_pClientSocket;
	EDecoder *decoder;
	struct read_buf *m_buf;
	bool m_needsWriteSelect;
};
