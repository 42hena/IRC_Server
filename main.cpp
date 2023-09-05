#include <string>

#include "IRC_server.h"

int main(int argc, char *argv[])
{
	IRC_server server(argc, argv);

	while (server.isRunStatus())
		;
}
