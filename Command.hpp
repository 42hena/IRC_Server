#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include "Define.hpp"
#include "Client.hpp"

class Command
{
public:
    void    User(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param);
    void    Nick(MAP<SOCKET, Client*>::iterator &iter, STRING &command, STRING param);

private:
    STRING  msg;
};

#endif
