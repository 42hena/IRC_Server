#include "server.hpp"

int	main(int argc, char *argv[])
{
	// 생성
	Server	irc_server(argc, argv);

    while (irc_server.IsRunningState())
        irc_server.Run();
}