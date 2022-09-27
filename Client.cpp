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
