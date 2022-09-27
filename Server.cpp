#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "Server.hpp"

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
	COUT << "IRC Server Desstructor" << LF;
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
	networkProcess();
	packetMarshalling();
	clientDisconnect();
}

// void	Server::networkClose(void)
// {
// 	close(listen_sock_);
// 	return ;
// }

int	Server::CalculateMaxFD(SOCKET sock)
{
	int	max_fd = sock;
	for (MAP<SOCKET, Client *>::iterator iter = clients.begin(); iter != clients.end(); iter++)
	{
		if (max_fd < iter->first)
			max_fd = iter->first;
	}
	return (max_fd);
}

void	Server::networkProcess(void)
{
	//FD ZERO
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);

	//listen socket add(read_set)
	FD_SET(serv_listen_sock, &read_set);

	//client socket add(read_set, write_set)
	for (MAP<SOCKET, Client *>::iterator iter = clients.begin()  ;
		iter != clients.end() ;
		iter++)
	{
		FD_SET(iter->first, &read_set);
		FD_SET(iter->first, &write_set);
	}

	// set timeout
	struct timeval	time_out;
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;

	// select(...)
	int	select_result = select(CalculateMaxFD(serv_listen_sock) + 1, &read_set, &write_set, NULL, &time_out);
	if (select_result > 0)
	{
		//new client
		if (FD_ISSET(serv_listen_sock, &read_set))
		{
			// acceptClient(serv_listen_sock);
			if (select_result == 1)
				return ;
		}

		//current client
		for (MAP<SOCKET, Client *>::iterator iter = clients.begin() ;
			iter != clients.end() ;
			iter++)
		{
			if (FD_ISSET(iter->first, &read_set))
				// recvPacket(iter);
			if ((iter->second->IsDisconnected() == false)
				&& FD_ISSET(iter->first, &write_set)
				&& iter->second->GetSendBuffer().length() > 0)
				;
				// sendPacket(iter);
		}
	}
}

bool	Server::IsRunningState(void)
{
	return status;
}
