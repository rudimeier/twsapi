/* Copyright (C) 2018 Ruediger Meier
 * Author:  Ruediger Meier <sweet_f_a@gmx.de>
 * License: BSD 3-Clause */

#pragma once

class EPosixClientSocket;
class EDecoder;
struct read_buf;

class TWSAPIDLLEXP EReaderST
{
public:
	EReaderST(EPosixClientSocket *clientSocket);
	~EReaderST(void);

	void select_timeout( int msec );
	void onReceive();
	void reset();

private:
	int readV100Plus();
	int decode_one_msg(const char* begin, uint32_t size);

	EPosixClientSocket *m_pClientSocket;
	EDecoder *decoder;
	struct read_buf *m_buf;
};
