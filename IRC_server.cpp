#include <iostream> // cout
#include <string> //string
#include <cstdlib> // atoi

#include "IRC_server.h"

// IRC_server's Constructor
IRC_server::IRC_server(int argc, char *argv[])
	: status(true)
{
	// check argc
	if (!IsArgcValid(argc))
	{
		std::cout << "./program_name port password 형식이 아닙니다.\n";
		
		// Replace the value of status with false.
		ChangeServerDown();
		return ;
	}

	// check argv
	if (!IsArgvValid(argv))
	{
		// Replace the value of status with false.
		ChangeServerDown();
		return ;
	}

	// Init Server Port
	SetServPort(atoi(argv[1]));

	// Init Server Password
	SetServPassword(argv[2]);

#ifdef DEBUG
	std::cout << GetServPort() << ' ' << GetServPassword() << '\n';
	std::cout << "Pass argc, argv ^.^\n";
#endif

	std::cout << "IRC_server's constructor\n";
}

// IRC_server's Destructor
IRC_server::~IRC_server()
{
	std::cout << "IRC_server's Destructor\n";
}

// --------------------
// --------------------
// --------------------

// check argc
bool IRC_server::IsArgcValid(int argc)
{
// -----
	return (argc == 3 ? true : false); 
}

// check argv
bool IRC_server::IsArgvValid(char* argv[])
{
	int inputPort;
	int inputPasswordSize;
// -----

	// convert port [char* -> int]
	inputPort = atoi(argv[1]);

	// check valid inputPort
	if (!IsPortValid(inputPort))
	{
		std::cout << "올바른 포트 번호 범위가 아닙니다."  << '\n';
		return (false);
	}

	// Get password's size
	inputPasswordSize = strlen(argv[2]);

	// check password size
	if (inputPasswordSize > 20)
	{
		std::cout << "비밀번호가 20글자를 초과합니다.\n";
		return (false);
	}

#ifdef P_DEBUG
	std::cout << "input port: [" << inputPort << "]\n";
	std::cout << "password size: [" << inputPasswordSize << "] password: [" << argv[2] <<"]\n";
#endif

	return (true);
}

// check port
bool IRC_server::IsPortValid(int convertPort)
{
// -----

	// port range(0 ~ 65535) but 
	if (convertPort > 0 && convertPort < 65536)
	{
	#ifdef P_DEBUG
		if (convertPort < 1024)
			std::cout << "Well known port\n";
		else if (convertPort < 49152)
			std::cout << "registered port\n";
		else
			std::cout << "dynamic port\n";
	#endif
		return (true);
	}
	else
		return (false);
}
// TODO: 0번 포트는 뭔지 자세히 찾아보기

void IRC_server::ChangeServerDown()
{
// -----
	status = false;
}
// TODO: bool이 아닌 enum으로 바꿔봅시다.

bool IRC_server::isRunStatus()
{
// -----
	return (status);
}// TODO: bool 자료형이 아닌 enum으로 바꿔봅시다.

// Get Server Port
unsigned short IRC_server::GetServPort()
{
//-----
	return port;
}

// Update Server
void IRC_server::SetServPort(int inputPort)
{
//-----

	// conver port [int -> unsigned short]
	port = inputPort;
}

std::string IRC_server::GetServPassword()
{
//-----
	return password;
}

void IRC_server::SetServPassword(char* inputPassword)
{
//-----

	// convert password [char* -> string]
	password = inputPassword;
}
