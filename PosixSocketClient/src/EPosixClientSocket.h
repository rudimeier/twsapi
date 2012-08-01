#ifndef eposixclientsocket_def
#define eposixclientsocket_def

#include "EClientSocketBase.h"

namespace IB {

class EWrapper;

class EPosixClientSocket : public EClientSocketBase
{
public:

	explicit EPosixClientSocket( EWrapper *ptr);
	~EPosixClientSocket();

	// override virtual funcs from EClient
	bool eConnect( const char *host, unsigned int port, int clientId=0);
	bool eConnect2( const char *host, unsigned int port, int clientId=0,
		int family=0 );
	void eDisconnect();

	bool isSocketOK() const;
	int fd() const;

	/* Here's the expert API that just performs protocol actions on
	 * a socket established and maintained elsewhere. */
	int handshake(int socket, int clientId = 0);
	int wavegoodbye(void);

private:

	int send( const char* buf, size_t sz);
	int receive( char* buf, size_t sz);

public:
	// callback from socket
	void onReceive();
	void onSend();

private:

	int m_fd;

	/* for the multi-stage handshake */
	enum {
		HND_SHK_ST_UNK,
		HND_SHK_ST_SENT_TOKEN,
		HND_SHK_ST_RCVD_CONNACK,
	} hnd_shk_state;
};

} // namespace IB
#endif
