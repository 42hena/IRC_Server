#include "Client.hpp"

Client::Client(SOCKET s, STRING hostName)
{
	client_sock = s;
	disconnect_flag = 0;
	join_flag =  0;
	host_name = hostName;
	return ;
}

bool Client::IsDisconnected()
{
    return disconnect_flag;
}

STRING& Client::GetSendBuffer()
{
    return send_buf;
}

STRING& Client::GetRecvBuffer()
{
    return recv_buf;
}

void   Client::SetDisconnect()
{
    disconnect_flag = true;
}

STRING		Client::GetUserInfo()
{
	STRING info = ":" + GetNickName() +"!" +GetUserName() +"@"+GetHostName();
	return info;
}		

STRING		Client::GetNickName()
{
	return nick_name;
}

STRING		Client::GetUserName()
{
	return user_name;
}
STRING		Client::GetHostName()
{
	return host_name;
}

unsigned char&		Client::GetJoinFlag()
{
	return join_flag;
}

void 				Client::setUserName(STRING &userName)
{
	user_name = userName;
	join_flag |= (1 << USER_SETTING);
}
void 				Client::setHostName(STRING &hostName)
{
	host_name = hostName;
}

void 				Client::setNickName(STRING &nickName)
{
	nick_name = nickName;
	join_flag |= (1 << NICK_SETTING);
}