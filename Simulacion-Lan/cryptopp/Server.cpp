#include <crypto++/cryptlib.h>
#include <cryptopp/base64.h>
#include "cryptopp/base64.h"
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;
using namespace CryptoPP;
int size = 512;


/*
Decodes a string thats encoded with base 64 cryptopp algorythm


*/
string decodeB64(string input){
  cout<<"llego a decode b64"<<endl;
  string in(input);

  string decoded;

  CryptoPP::StringSource ss(
    in,
    true,
    new CryptoPP::Base64Decoder(
        new CryptoPP::StringSink(decoded)
    )
  );

  return decoded;
}


/*
Receives a const * char with the name of a file. It reads the file that should contain text encrypted with a
cryptopp base64 algorythm and sends it to decode.
*/
int decodeFile(char * name){
  cout<<"llego a decode"<<endl;
  int cnt = 0;
  FILE * inputFile = fopen(name, "r");
  FILE * outputFile = fopen("decoded.txt", "w");
  string decoded;
  char * input = (char*)malloc(size);
  while (1){
    cnt = fread(input, sizeof(char), size, inputFile);
    if (cnt == 0) break;
    decoded = decodeB64(input);
    fwrite(decoded.c_str(), sizeof(char),strlen(decoded.c_str()), outputFile);
  }

  fclose(inputFile);
  fclose(outputFile);
  return 0;
}

int main(){
  int client, server;
  int portNum = 1500;
  int bufsize = 512;
  char buffer[bufsize];
	int cont;

  struct sockaddr_in server_addr;
  socklen_t size;

  //creates a soceket.
  client = socket(AF_INET, SOCK_STREAM, 0);

  //makes sure socket is done right
  if (client < 0){
  	printf("Error establishing socket...\n");
    exit(1);
  }


  printf("Socket Server has been created \n");


  //socket parameters
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(portNum);


  //it starts the bind
  if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0){
  	printf("Error binfin connection the socket has already been established...\n");
    return -1;
  }


  size = sizeof(server_addr);
  printf("Looking for clients...\n");

  //it starts listening to another socket.
  listen(client, 1);



  int clientCount = 1;

  //accepts the other Socket
  server = accept(client,(struct sockaddr *)&server_addr,&size);


  if (server < 0)
  	printf("Error on accepting\n");

/*
  Receives a file from the client side and writes it in another file.

*/

	FILE* receivedFile = fopen("received.txt", "w");
	FILE * decodedFile = fopen ("decoded.txt", "w");
	int st;
	memset(buffer, 0, bufsize * sizeof(char));
 	while((st = read(server, buffer, bufsize)) > 0){
    cout<<"St es: "<<st<<endl;
		fwrite(buffer, sizeof(char),st, receivedFile);
		printf("entro \n");
		printf("recibi %s \n", buffer);
	}

  fclose(receivedFile);

  //sends the file to be decoded
  decodeFile("received.txt");

  //sends ack to let know the client that it finished.
  char * bufferino = "ACK";
  send(server,bufferino,bufsize,0);

  close(client);
  return 0;
}
