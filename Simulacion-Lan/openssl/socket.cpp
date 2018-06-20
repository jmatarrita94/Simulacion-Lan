#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>


int main(int argc, char* argv[])
{

    int client, server;
    int portNum = 1500; // NOTE that the port number is same for both client and server
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];


    struct sockaddr_in server_addr;
    socklen_t size;

    client = socket(AF_INET, SOCK_STREAM, 0);

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    if (client < 0)
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket server has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);





    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }


    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;


    listen(client, 1);



    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    if (server < 0)
        cout << "=> Error on accepting..." << endl;

    //strcpy(buffer, "=> Server connected...\n");
    send(server, buffer, bufsize, 0);


    return 0;
}
