#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <queue>



using namespace std;
struct ClntInfo{
    int sockfd;
    std::string ip;
    uint16_t port;
};


class Socket
{
public:
    Socket();
    int Bind(string ip, uint16_t port);
    int Listen(int backlog);
    int Accept(string&ip, uint16_t&port);
    int Send(int sockfd, string msg, int flag);
    int Recv(int sockfd, string& msg, int flag);
    int Shutdown(int fd);
    int Close(void);

    ClntInfo GetClntInfo();

    vector<std::string> GetHostAddr(string dev);

private:
    int m_sockfd;
    queue<ClntInfo> m_qClntSock;
};

#endif // SOCKET_H
