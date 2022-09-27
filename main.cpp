#include "Server.hpp"

int	main(int argc, char *argv[])
{
	Server	irc_server(argc, argv);

    while (irc_server.IsRunningState())
        irc_server.Run();
}