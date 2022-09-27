#ifndef SERVER_HPP
# define SERVER_HPP

# include "Define.hpp"
# include <string>
# include "Client.hpp"
# include <netinet/in.h>

class Server
{
public:
	Server(int argc, char *argv[]);
	~Server(void);

	// 네트워크 초기화
	void	NetworkInit();
	
	// Run
	void	Run();
	void	networkProcess();
	void	packetMarshalling();
	void	clientDisconnect();

	// 상태 체크
	bool	IsRunningState(void);
	
	int		CalculateMaxFD(SOCKET sock);

private:
	//config value
	STRING								irc_login_password;

	//network
	unsigned short						serv_port;
	struct	sockaddr_in					s_addr_in;
	unsigned int						sock_count;

	//server info
	bool								status;
	SOCKET								serv_listen_sock;

	//client
	fd_set								read_set;
	fd_set								write_set;
	MAP<SOCKET, Client *>				clients;
};

#endif
