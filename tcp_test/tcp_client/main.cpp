#include <iostream>
#include "Socket.h"
#include <unistd.h>

using namespace std;

int main(int argc, char * argv[])
{
    cout << "Hello World!" << endl;
    Socket clnt;
    if (clnt.Connet(argv[1], 6666))
    {
        perror("connet");
        return -1;
    }

    while (true) {

        string input;
        cin >> input;
        if (input == "bye")
            break;
        cout << input << endl;
        if (input == "clntshut") break;
        clnt.Send(input, 0);
//        sleep(1);
    }

    clnt.Shutdown();
    sleep(100);
    clnt.Close();
    return 0;
}
