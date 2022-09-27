#include <iostream>

#include "Command.hpp"
#include "Client.hpp"
#include "ParseUtil.hpp"


#include <sys/socket.h>

void Command::User(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param)
{
    msg = "";
    COUT << "User Command\n";
	COUT << "Command : [" << command << "], Param : [" << param << "]\n";

	VECTOR<STRING> paramVector = split(param, ' ');

    for (unsigned int i = 0 ;i < paramVector.size() ; ++i)
    {
        COUT << paramVector[i] << '\n';
    }

	iter->second->setUserName(paramVector[0]);
	iter->second->setHostName(paramVector[2]);
    // real Nick


    STRING user_info = iter->second->GetUserInfo();
    if ((iter->second->GetJoinFlag() & 6) == 6)
    {
        // std::string	user_info = iter->second->GetUserInfo();
        std::string tmp = ":irc.local 001 " + iter->second->GetUserInfo() + " :Welcome to the ft_irc Network";
        tmp += CRLF;

        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
        tmp = user_info + " 002 " + iter->second->GetNickName() + " :Your host is fortytwitch, running version 1.0\r\n";
        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
        tmp = user_info + " 003 " + iter->second->GetNickName() + " :This server was created in August 30th\r\n";
        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
        tmp = user_info + " 004 " + iter->second->GetNickName() + " :fortytwitch 1.0 o o]\r\n";
        send(iter->first, tmp.c_str(), tmp.length(), MSG_DONTWAIT);
	     
    }
}