#include "server.hpp"
#include <arpa/inet.h>
#include <cctype>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstring>
#include <sstream>
#include <vector>

#include <iostream>

Server::Server(int argc, char *argv[])
	: irc_login_password(""), serv_port(6667), status(true)
{
#ifdef DEBUG
	COUT << "now argc              [" << argc << "]\n";
	for (size_t i =0 ; i < argc ; ++i)
	{
		COUT << "now port              [" << argv[i] << "]\n";	
	}
	COUT << "now irc_login_password[" << irc_login_password << "]\n";
#endif

	// TODO:: 삭제
	status = true;
	serv_port = 6667;
	(void)argc;
	(void)argv;
}

Server::~Server(void)
{
}
