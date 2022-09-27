#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "Define.hpp"

class Client
{
public:
	Client(SOCKET s, std::string host_name);
	~Client();

	// disconnect flag
	bool				IsDisconnected();


	// Buffer
	STRING&				GetSendBuffer();
	STRING&				GetRecvBuffer();

	void				SetDisconnect();
	

private:
    bool				disconnect_flag;
    STRING				send_buf;
	STRING				recv_buf;
	int					client_sock;
	unsigned char 		join_flag;
	STRING				host_name;
};

#endif
