#include <b64/encode.h>
#include <b64/decode.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#define SIZE 512
using namespace std;

/*Decodes a file that uses libb64 base64 algorythm*/
void decode(FILE* inputFile, FILE* outputFile)
{
	base64::decoder E;
	/* set up a destination buffer large enough to hold the decoded data */
	int size = SIZE;
	char* encoded = (char*)malloc(size);
	char* decoded = (char*)malloc(1*size); /* ~3/4 x encoded */
	/* we need an encoder and decoder state */
	/* store the number of bytes encoded by a single call */
	int cnt = 0;

	/* gather data from the input and send it to the output */
	while (1)
	{
		cnt = fread(encoded, sizeof(char), size, inputFile);
		if (cnt == 0) break;
		cnt = E.decode(encoded, sizeof(encoded), decoded);
		/* output the encoded bytes to the output file */
		fwrite(decoded, sizeof(char), cnt, outputFile);
	}
	/*---------- START DECODING  ----------*/

	free(encoded);
	free(decoded);
}


/*
	handles all the logic of creating a server and conecting it to the client plus send the encoded file to decoded
	and sends confirmation to the client side.
*/
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
    while ( (st = read( server, buffer, bufsize )) > 0 ) {	// Read returns EOF when we shutdown
      fwrite(buffer, sizeof(buffer), bufsize, receivedFile);				// the writing socket in client
    }

		char * bufferino = "ACK";
	  send(server,bufferino,bufsize,0);

  /*  FILE* output = fopen("Message.txt", "w");
    decode(receivedFile, output);*/


    close(client);
    fclose(receivedFile);
    //fclose(output);
    return 0;
}

int main (){

  if (doServer() == 0){
    cout<<"funciono"<<endl;
  }else{
    cout<<"no funciono"<<endl;
  }
  return 0;
}
