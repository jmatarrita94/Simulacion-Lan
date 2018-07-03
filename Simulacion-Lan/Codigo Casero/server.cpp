#include "base64.cpp"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Chrono.cpp"

#define SIZE 512
using namespace std;

/*Decodes a file that uses libb64 base64 algorythm*/
void decode(FILE* inputFile, FILE* outputFile)
{
	cout<<"llego"<<endl;
	/* set up a destination buffer large enough to hold the decoded data */
	int size = SIZE;
	char* encoded = (char*)malloc(size);
	string decoded ; /* ~3/4 x encoded */
	/* we need an encoder and decoder state */
	/* store the number of bytes encoded by a single call */
	int cnt = 0;

	/* gather data from the input and send it to the output */
	while (1)
	{
		cnt = fread(encoded, sizeof(char), size, inputFile);
		cout<<"llego al while y encode es"<<cnt<<endl;
		if (cnt == 0) break;
		decoded = base64_decode(encoded);
		cout<<"llego y decoded es: "<<decoded<<endl;
		/* output the encoded bytes to the output file */
		fwrite(decoded.c_str(), sizeof(char), strlen(decoded.c_str()), outputFile);
	}
	/*---------- START DECODING  ----------*/



}


int doServer(){
    int client, server;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 512;
    char buffer[bufsize];
    int st;

    struct sockaddr_in server_addr;
    socklen_t size;


    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0){
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

    FILE * receivedFile = fopen("received.txt", "w");
    memset(buffer, 0, bufsize * sizeof(char));
    if ( (st = read( server, buffer, bufsize )) > 0 ) {	// Read returns EOF when we shutdown
			cout<<"Buffer es:"<<buffer<<endl;
      fwrite(buffer, sizeof(char),strlen(buffer), receivedFile);				// the writing socket in client

    }

		fclose(receivedFile);
		cout<<"llego"<<endl;

		FILE* output = fopen("Message.txt", "w");
		receivedFile = fopen("received.txt", "r");
    decode(receivedFile, output);

		char * bufferino = "ACK";
	  send(server,bufferino,bufsize,0);


    close(client);
    fclose(receivedFile);
    //fclose(output);
    return 0;
}


int main(){
  doServer();
}
