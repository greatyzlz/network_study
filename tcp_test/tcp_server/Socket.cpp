#include "Socket.h"
#include <assert.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <vector>
#include <iostream>

using namespace std;

Socket::Socket()
{

    m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(m_sockfd != -1);

}

int Socket::Bind(string ip, uint16_t port)
{
    struct sockaddr_in in_addr;
    bzero((void*)&in_addr, sizeof(struct sockaddr_in));
//    in_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    inet_pton(AF_INET, ip.c_str(), &in_addr.sin_addr.s_addr);
    in_addr.sin_port = htons(port);
    in_addr.sin_family = AF_INET;
    return bind(m_sockfd, (struct sockaddr*)&in_addr, sizeof(struct sockaddr));
}

int Socket::Listen(int backlog)
{
    return listen(m_sockfd, backlog);
}

int Socket::Accept(string &ip, uint16_t &port)
{
    struct sockaddr addr;
    bzero((void*)&addr, sizeof(struct sockaddr));
    socklen_t addr_len = 100;
    int new_fd = accept(m_sockfd, &addr, &addr_len);
    struct sockaddr_in * in_addr = (struct sockaddr_in *)&addr;
    ip = inet_ntoa(in_addr->sin_addr);
    port = ntohs(in_addr->sin_port);
    assert(new_fd > 0);
    ClntInfo clntinfo = {new_fd, ip, port};
    m_qClntSock.push(clntinfo);
    return new_fd;
}

int Socket::Send(int sockfd, string msg, int flag)
{
    return send(sockfd, (void*)msg.c_str(), msg.length(), flag);
}

int Socket::Recv(int sockfd, string &msg, int flag)
{
    int maxlen = 4096;
    unsigned char buf[4096];
    memset(buf, 0, sizeof(buf));
    int ret = recv(sockfd, buf, maxlen, flag);
//    printf("%s\n", buf);
    msg = string(reinterpret_cast<const char*>(buf));
    return ret;
}

int Socket::Shutdown(int fd)
{
    return shutdown(fd, SHUT_WR);
}

int Socket::Close()
{
    return close(m_sockfd);
}

ClntInfo Socket::GetClntInfo()
{
    ClntInfo clntinfo;
    if (m_qClntSock.empty()) return clntinfo;
    clntinfo = m_qClntSock.front();
    m_qClntSock.pop();

    return clntinfo;
}

vector<std::string> Socket::GetHostAddr(string dev)
{
    vector<std::string> vIp;
    int fd, retn = 0;
    struct ifreq addrs[INET_ADDRSTRLEN];
    struct ifconf ifc;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return vIp;
    }

    ifc.ifc_len = sizeof(addrs);
    ifc.ifc_buf = (caddr_t)addrs;
    retn = ioctl(fd, SIOCGIFCONF, (void*)&ifc);
    if (retn)
    {
        perror("ioctl");
        return vIp;
    }

    for (unsigned long i = 0; i < ifc.ifc_len / sizeof(struct ifreq); ++i)
    {
        if (!ioctl(fd, SIOCGIFADDR, (void*)&addrs[i]))
        {
            if (!strcmp(addrs[i].ifr_ifrn.ifrn_name, dev.c_str()))
            {
//                cout << inet_ntoa(((struct sockaddr_in*)(&addrs[i].ifr_addr))->sin_addr) << endl;
                vIp.push_back(inet_ntoa(((struct sockaddr_in*)(&addrs[i].ifr_addr))->sin_addr));
            }
        }
    }

    return vIp;
}
