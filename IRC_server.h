#ifndef __IRC_SERVER_H__
#define __IRC_SERVER_H__

class IRC_server
{
public:
    IRC_server(int argc, char *argv[]);
    ~IRC_server();

private:
    IRC_server(/* args */);


    // --------------------
    std::string  password;
    int     port;
    int     sock_cnt;
};





#endif