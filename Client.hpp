#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

#include "Define.hpp"

class Channel;

class Client
{
public:
	Client(SOCKET s, std::string host_name);
	~Client();

	MAP<STRING, Channel *> * channel_ptr;

	// disconnect flag
	bool				IsDisconnected();


	// Buffer
	STRING&				GetSendBuffer();
	STRING&				GetRecvBuffer();

	void				SetDisconnect();
	
	// setting
	void 				setNickName(STRING &);
	void 				setUserName(STRING &);
	void 				setHostName(STRING &);
	

	// getting
	unsigned char&		GetJoinFlag();
	STRING				GetUserInfo();
	STRING				GetNickName();
	STRING				GetUserName();
	STRING				GetHostName();

private:
    bool				disconnect_flag;
    STRING				send_buf;
	STRING				recv_buf;
	int					client_sock;
	unsigned char 		join_flag;

	STRING				nick_name;
	STRING				user_name;
	STRING				host_name;
};

#endif
