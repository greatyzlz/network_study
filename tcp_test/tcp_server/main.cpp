#include <iostream>
#include "Socket.h"
#include <time.h>
#include <unistd.h>
#include <thread>

void process_data(void * data)
{
    Socket * serv = reinterpret_cast<Socket*>(data);
    ClntInfo clntinfo = serv->GetClntInfo();
    while (true) {
        string msg;
        if (serv->Recv(clntinfo.sockfd, msg, 0) <= 0)
        {
            perror("recv");
            break;
        }

        if (msg == "shutdown")
        {
            if (serv->Shutdown(clntinfo.sockfd)) perror("shutdown");
            break;
        }
        if (msg == "bye")
        {
            cout << "client say bye !!!" << endl;
            break;
        }

        cout << clntinfo.ip <<  " --> " << "192.168.244.144" << " : " << msg << endl;
    }


}

using namespace std;

int main()
{
    Socket serv;
    vector<std::string> vIp = serv.GetHostAddr("ens33");
    if (vIp.empty())
        return -1;

    if(serv.Bind(vIp[0], 6666)) {
        perror("bind");
        return -1;
    }



    while (true) {
        if (serv.Listen(10)) perror("listen");
        string ip;
        uint16_t port;

        if (serv.Accept(ip, port) < 0)
        {
            perror("accept");
            return -1;
        }
        cout << "accept client: " << ip << ":" << port << endl;

        thread thd(process_data, &serv);
        thd.detach();
    }

    serv.Close();
    return 0;
}
