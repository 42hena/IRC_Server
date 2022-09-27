#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "Server.hpp"
#include "ParseUtil.hpp"


Server::Server(int argc, char *argv[])
	: irc_login_password(""), serv_port(6667), status(true), sock_count(0)
{
	COUT << "IRC Server Constructor" << LF;
	serv_port = 6667;
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
	int retval;

	// socket
	serv_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_listen_sock == -1)
	{
		status = false;
		return ;
	}

	// set listen_sock_ O_NONBLOCK
	fcntl(serv_listen_sock, F_SETFL, O_NONBLOCK);

	// set listen_sock_ SO_REUSEADDR
	//struct timeval optval = {0, 1000};
	int optval = 1;
	retval = setsockopt(serv_listen_sock, SOL_SOCKET, SO_REUSEADDR,
		&optval, sizeof(optval));
	if (retval == -1)
	{
		status = false;
		return ;
	}

	// unset Nagle algorithm
	//int optval = 1;
	retval = setsockopt(serv_listen_sock, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
	if (retval == -1)
	{
		status = false;
		return ;
	}

	// bind
	memset(&s_addr_in, 0x00, sizeof(s_addr_in));
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr_in.sin_port = htons(serv_port); // SERVERPORT
	retval = bind(serv_listen_sock, reinterpret_cast<struct sockaddr *>(&s_addr_in), sizeof(s_addr_in));
	if (retval == -1)
	{
		status = false;
		return ;
	}

	// listen
	retval = listen(serv_listen_sock, SOMAXCONN);
	if (retval == -1)
	{
		status = false;
		return ;
	}

	//client count
	sock_count += 1;

	std::cout << "IRC Server started" << std::endl;
	return ;
}

void	Server::Run()
{
	// COUT << "RUNNING" << LF;
	NetworkProcess();
	PacketMarshalling();
	// clientDisconnect();
}

// void	Server::networkClose(void)
// {
// 	close(listen_sock_);
// 	return ;
// }

int	Server::CalculateMaxFD(SOCKET sock)
{
	int	max_fd = sock;
	for (MAP<SOCKET, Client *>::iterator iter = clients.begin(); 
		iter != clients.end(); 
		iter++
		)
	{
		if (max_fd < iter->first)
			max_fd = iter->first;
	}
	return (max_fd);
}

void	Server::NetworkProcess(void)
{
	//FD ZERO
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);

	//listen socket add(read_set)
	FD_SET(serv_listen_sock, &read_set);

	//client socket add(read_set, write_set)
	for (MAP<SOCKET, Client *>::iterator iter = clients.begin(); 
			iter != clients.end(); 
			iter++
		)
	{
		FD_SET(iter->first, &read_set);
		FD_SET(iter->first, &write_set);
	}

	//set timeout
	struct timeval	time_out;
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;

	//select(...)
	int	select_result = select(CalculateMaxFD(serv_listen_sock) + 1, &read_set, &write_set, NULL, &time_out);
	if (select_result > 0)
	{
		//new client
		if (FD_ISSET(serv_listen_sock, &read_set))
		{
			AcceptClient(serv_listen_sock);
			if (select_result == 1)
				return ;
		}
		//current client
		for (MAP<SOCKET, Client *>::iterator iter = clients.begin(); 
				iter != clients.end(); 
				iter++
			)
		{
			if (FD_ISSET(iter->first, &read_set))
				recvPacket(iter);
			if ((iter->second->IsDisconnected() == false) \
			&& FD_ISSET(iter->first, &write_set) \
			&& iter->second->GetSendBuffer().length() > 0)
				sendPacket(iter);
		}
	}
	return ;
}

bool	Server::IsRunningState(void)
{
	return status;
}

void	Server::recvPacket(std::map<SOCKET, Client *>::iterator &iter)
{
	unsigned char	buf[BUFFER_MAX];

	int	recv_ret = recv(iter->first, reinterpret_cast<void *>(buf), sizeof(buf), 0);
	if (recv_ret == 0)
		iter->second->SetDisconnect();
	else if (recv_ret > 0)
	{
		buf[recv_ret] = '\0';
		iter->second->GetRecvBuffer().append(reinterpret_cast<char *>(buf));
	}
	return ;
}

void	Server::sendPacket(MAP<SOCKET, Client *>::iterator &iter)
{
	unsigned char	buf[BUFFER_MAX];

	memcpy(buf, iter->second->GetSendBuffer().c_str(), iter->second->GetSendBuffer().length() + 1);

	int	send_ret = send(iter->first, reinterpret_cast<void *>(buf), strlen(reinterpret_cast<char *>(buf)), 0);
	if (send_ret == -1)
		iter->second->SetDisconnect();
	else if (send_ret > 0)
		iter->second->GetSendBuffer().erase(0, send_ret);
}

void	Server::PacketMarshalling()
{
	for (MAP<SOCKET, Client *>::iterator iter = clients.begin() ;
			iter != clients.end() ;
			iter++
		)
	{
		if (iter->second->GetRecvBuffer().length() != 0)
			PacketAnalysis(iter);
	}
}


void	Server::PacketAnalysis(std::map<SOCKET, Client *>::iterator& iter)
{
	VECTOR<STRING> Packets;
	std::string	packet_buf;
	std::string crlf = "\r\n";

	ParsePacket(iter->second->GetRecvBuffer(), crlf, Packets);



	for (unsigned int i = 0 ; i < Packets.size() ; ++i)
	{
		STRING	command;
		STRING	param;
		if (Packets[i].find(' ') != STRING::npos)
		{
			command = Packets[i].substr(0, Packets[i].find(' '));
			param = Packets[i].substr(Packets[i].find(' ') + 1);
		}
		else
		{
			command = Packets[i];
			param = "";
		}
		std::cout << "--requestCommand-- [command : " << command << ']' << ", " << "[param : " << param << ']' << '\n';
		// requestCommand(iter, command, param);
	}

	return ;
}

void	Server::AcceptClient(SOCKET listen_sock)
{
	SOCKET				client_sock;
	struct sockaddr_in	c_addr_in;
	socklen_t			c_addr_len = sizeof(c_addr_in);

	memset(&c_addr_in, 0x00, sizeof(c_addr_in));
	//accept(...)
	client_sock = accept(listen_sock, reinterpret_cast<struct sockaddr *>(&c_addr_in), &c_addr_len);
	if (client_sock == -1)
		return ;

	//if current client's counts are over select function's set socket max counts, accepted socket is closed.
	if (sock_count >= FD_SETSIZE)
	{
		close(client_sock);
		return ;
	}

	//set client_sock O_NONBLOCK
	fcntl(client_sock, F_SETFL, O_NONBLOCK);

	//create client session
	Client*	new_client = new Client(client_sock, inet_ntoa(c_addr_in.sin_addr));

	//push client socket
	clients.insert(std::make_pair(client_sock, new_client));

	//client count
	sock_count += 1;

	//display client network info
	std::cout << "-------------------" << std::endl;
	std::cout << "client connected" << std::endl;
	std::cout << "client socket : " << client_sock << std::endl;
	std::cout << "client port   : " << ntohs(c_addr_in.sin_port) << std::endl;
	std::cout << "client ip     : " << inet_ntoa(c_addr_in.sin_addr) << std::endl;
	std::cout << "-------------------" << std::endl;
	return ;
}