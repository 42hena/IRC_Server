#ifndef SERVER_HPP
# define SERVER_HPP

# include "Define.hpp"
# include <string>
# include <netinet/in.h>

class Server
{
private:
	//config value
	STRING								irc_login_password;

	//network
	unsigned short						serv_port;

	//server info
	bool								status;
	

public:
	Server(int argc, char *argv[]);
	~Server(void);

	// 상태 체크
	bool	IsRunningState(void);

private:

};
#endif
