#include "Socket.h"
#include <assert.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>

Socket::Socket()
{

    m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(m_sockfd != -1);

}

int Socket::Connet(string ip, uint16_t port)
{
    struct sockaddr_in in_addr;
    bzero((void*)&in_addr, sizeof(struct sockaddr_in));
//    in_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    inet_pton(AF_INET, ip.c_str(), &in_addr.sin_addr.s_addr);
    in_addr.sin_port = htons(port);
    in_addr.sin_family = AF_INET;

    return connect(m_sockfd, (struct sockaddr*)&in_addr, sizeof(struct sockaddr));
}



int Socket::Send(string msg, int flag)
{
    return send(m_sockfd, (void*)msg.c_str(), msg.length(), flag);
}

int Socket::Recv(string& msg, int flag)
{
    int maxlen = 4096;
    unsigned char buf[4096];
    int ret = recv(m_sockfd, buf, maxlen, flag);
    msg.copy((char*)buf, maxlen);
    return ret;
}

int Socket::Shutdown()
{
    return shutdown(m_sockfd, SHUT_WR);
}

int Socket::Close()
{
    return close(m_sockfd);
}
