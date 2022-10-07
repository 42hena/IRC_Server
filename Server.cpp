#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "Channel.hpp"
#include "Command.hpp"

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
		requestCommand(iter, command, param);
	}

	return ;
}

void NickCommand(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param)
{
	STRING msg = "";
    COUT << "Nick Command\n";
	COUT << "Command : [" << command << "], Param : [" << param << "]\n";

	VECTOR<STRING> paramVector = split(param, ' ');
	iter->second->setNickName(paramVector[0]);
}

void UserCommand(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param)
{
    COUT << "User Command\n";
	COUT << "Command : [" << command << "], Param : [" << param << "]\n";

	VECTOR<STRING> paramVector = split(param, ' ');

	iter->second->setUserName(paramVector[0]);
	iter->second->setHostName(paramVector[2]);
    // real Nick


    STRING user_info = iter->second->GetUserInfo();
    if ((iter->second->GetJoinFlag() & 6) == 6)
    {
        // std::string	user_info = iter->second->GetUserInfo();
		// STRING info = iter->second->GetNickName()+"!"+iter->second->GetUserName() +"@"+iter->second->GetHostName();
		STRING info = iter->second->GetNickName();
        STRING tmp = ":IrcServer 001 " + info + " :fuck\r\n";
		COUT << tmp << '\n';
        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
        tmp = ":IrcServer 002 " + info + " :Your host is fortytwitch, running version 1.0\r\n";
		COUT << tmp << '\n';
        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
        tmp = ":IrcServer 003 " + info + " :This server was created in August 30th\r\n";
		COUT << tmp << '\n';
        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
        tmp = ":IrcServer 004 " + info + " :fortytwitch 1.0 o o]\r\n";
		COUT << tmp << '\n';
        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
    }
	command = "";
	param = "";
}

void	Server::requestCommand(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	// if (command == "PASS")
	iter->second->GetRecvBuffer() = "";
	if (command == "NICK")
		NickCommand(iter, command, param);
	else if (command == "USER")
		UserCommand(iter, command, param);
	else if (command == "JOIN")
		Join(iter, command, param);
	else if (command == "PART")
		Part(iter, command, param);
	else
		COUT << "NO\n";
	command = "";
	param = "";
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

// TEST

void TEST(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param)
{
	std::string host = ":IrcServer";
	STRING info = iter->second->GetUserInfo();
	std::string re1 = info + " " + command + " :" +param + "\r\n";
	send(iter->first, re1.c_str(), re1.length(), MSG_DONTWAIT);
	//TODO:: host
	//topic이 있을 시에 전달하기.
	std::string re2 = host + " 353 " + iter->second->GetNickName() + " = " + param + " :";

	STRING name_list = "@" + iter->second->GetNickName();
	re2 += name_list += "\r\n";
	COUT << re2 << '\n';
	send(iter->first, re2.c_str(), re2.length(), MSG_DONTWAIT);
	

	std::string re3 = host + " 366 " + iter->second->GetNickName() + " " + param + " :End of /NAMES list.\r\n";
	
	COUT << re3 << '\n';
	send(iter->first, re3.c_str(), re3.length(), MSG_DONTWAIT);
}

void	Server::Join(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param)
{
    VECTOR<STRING> server_vector = split(param, ',');

	// ,으로 구분해주고 일일이 넣어줘야한다.
	for (unsigned int i = 0 ; i < server_vector.size() ; ++i)
	{
		std::cout << "Channel Name: [" << server_vector[i] << "]\n";
		MAP<STRING, Channel*>::iterator chan_iter = channels.find(server_vector[i]);
		
		// 이름이 존제x
		if (chan_iter == channels.end())
		{
			std::cout << "make channel\n";
			Channel* new_chann = new Channel(server_vector[i]);
			channels.insert(std::make_pair(server_vector[i], new_chann));

			// new_chann->assignOper(iter->second->GetNickName(), iter->second);
			new_chann->assignUser(iter->second->GetNickName(), iter->second);
		}
		// 찾았을 경우
		else
		{
			std::cout << "already channel exist\n";
			Channel* channel_info = chan_iter->second;
			// if (!channel_info->getUsers_().size())
			// 	channel_info->assignOper(iter->second->GetNickName(), iter->second);
			channel_info->assignUser(iter->second->GetNickName(), iter->second);
		}

		// 들어갔다라는 메세지 주기
		TEST(iter, command, server_vector[i]);
	}
	(void)command;
}


void	Server::requestPartMsg(MAP<SOCKET, Client*>::iterator &iter, \
						STRING& command, std::string& param)
{
    // nick!user@host
    STRING msg = "";
    for (std::map<SOCKET, Client*>::iterator c_iter = clients.begin(); c_iter != clients.end(); c_iter++)
    {
        msg = iter->second->GetUserInfo()+ " "
            + command + " :" + param + "\r\n"; 
        std::cout << msg << '\n';
		send(iter->first, msg.c_str(), msg.length(), MSG_DONTWAIT);
        // insertSendBuffer(c_iter->second, msg);
    }
}


void Server::Part(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param)
{
    // #체널 + 메세지 올 경우    
    VECTOR<STRING> splitted_param = split(param, ' ');

    STRING channel_name = splitted_param[0];
    MAP<STRING, Channel *>::iterator channel_iter = channels.find(channel_name);

    // 없다면 part할 필요가 없음
    if (channel_iter == channels.end())
        return ;
    
    // 나갈 친구의 nick
    STRING partNick = iter->second->GetNickName();

    std::cout << "PART TEST START\n";
    std::cout << "param :  [" << channel_name << "]\n";
    std::cout << "partSocket :  [" << partNick << "]\n";

    MAP<STRING, Client*> client_map = channel_iter->second->getUsers_();
    MAP<STRING, Client*>::iterator client_iter = client_map.find(partNick);
    // 내가 찾는 친구가 없네? 그럼 끝
    if (client_iter == client_map.end())
        return ;
    
	// 나가기
    channel_iter->second->eraseUser(partNick);
    // channel_iter->second->eraseOper(partNick);

    // 명령어 요청
    requestPartMsg(iter , command, param);
    // todo: part leaving message
}