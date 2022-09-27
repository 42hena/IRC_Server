#include <iostream>

#include "Command.hpp"
#include "Client.hpp"
#include "ParseUtil.hpp"


#include <sys/socket.h>

void Command::Nick(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param)
{
    msg = "";
    COUT << "Nick Command\n";
	COUT << "Command : [" << command << "], Param : [" << param << "]\n";

	VECTOR<STRING> paramVector = split(param, ' ');

    for (unsigned int i = 0 ;i < paramVector.size() ; ++i)
    {
        COUT << paramVector[i] << '\n';
    }

	iter->second->setNickName(paramVector[0]);
    // real Nick
}