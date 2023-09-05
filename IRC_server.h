#ifndef __IRC_SERVER_H__
#define __IRC_SERVER_H__

class IRC_server
{
public:
	IRC_server(int argc, char *argv[]);
	~IRC_server();

	bool isRunStatus();

private:
	IRC_server();

	bool IsArgcValid(int argc);
	bool IsArgvValid(char* argv[]);
	void ChangeServerDown();
	bool IsPortValid(int ret_atoi);

	unsigned short GetServPort();
	void SetServPort(int port);

	std::string GetServPassword();
	void SetServPassword(char* inputPassword);

	bool				status;
	unsigned short		port;
	std::string			password;
};

#endif
