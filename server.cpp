#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>

#include "server.hpp"

Server::Server(int argc, char *argv[])
	: irc_login_password(""), serv_port(6667), status(true), sock_count(0)
{
	COUT << "IRC Server Constructor" << LF;

#ifdef DEBUG
	COUT << "now argc              [" << argc << "]\n";
	for (size_t i =0 ; i < argc ; ++i)
	{
		COUT << "now port              [" << argv[i] << "]\n";	
	}
	COUT << "now irc_login_password[" << irc_login_password << "]\n";
#endif
	NetworkInit();
}

Server::~Server(void)
{
}

void	Server::NetworkInit()
{
	int ret;

	COUT << "NetworkInit" << LF;

	// socket
	serv_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_listen_sock == -1)
	{
		status = false;
		return ;
	}

	fcntl(serv_listen_sock, F_SETFL, O_NONBLOCK);

	int optval = 1;
	ret = setsockopt(serv_listen_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	
	// serv_listen_sock 초기화
	memset(&s_addr_in, 0x00, sizeof(s_addr_in));
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr_in.sin_port = htons(serv_port);

	ret = bind(serv_listen_sock, reinterpret_cast<sockaddr *>(&s_addr_in), sizeof(s_addr_in));
	if (ret == -1)
	{
		status = false;
		return ;
	}

	// listen
	ret = listen(serv_listen_sock, SOMAXCONN);
	if (ret == -1)
	{
		status = false;
		return ;
	}

	//client count 
	sock_count += 1;
}

void	Server::Run()
{
	COUT << "RUNNING" << LF;
	// networkProcess();
	// packetMarshalling();
	// clientDisconnect();
}

bool	Server::IsRunningState(void)
{
	return status;
}