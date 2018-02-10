﻿#pragma once

class RudiClient;
class EDecoder;
struct read_buf;

class TWSAPIDLLEXP RudiReader
{
public:
	RudiReader(RudiClient *clientSocket);
	~RudiReader(void);

	bool processNonBlockingSelect();
	void onSend();
	void onReceive();

private:
	void readV100Plus();

	RudiClient *m_pClientSocket;
	EDecoder *decoder;
	struct read_buf *m_buf;
	bool m_needsWriteSelect;
};