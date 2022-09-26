#include <iostream>

#include "server.hpp"


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
}

Server::~Server(void)
{
}


bool	Server::IsRunningState(void)
{
	return status;
}