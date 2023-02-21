#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>

using namespace std;

class Socket
{
public:
    Socket();
    int Connet(string ip, uint16_t port);
    int Send(string msg, int flag);
    int Recv(string& msg, int flag);
    int Shutdown(void);
    int Close(void);

private:
    int m_sockfd;
};

#endif // SOCKET_H
