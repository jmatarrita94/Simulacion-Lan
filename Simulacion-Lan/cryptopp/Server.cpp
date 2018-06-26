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


  client = socket(AF_INET, SOCK_STREAM, 0);

  if (client < 0){
  	printf("Error establishing socket...\n");
    exit(1);
  }


  printf("Socket Server has been created \n");


  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(portNum);



  if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0){
  	printf("Error binfin connection the socket has already been established...\n");
    return -1;
  }


  size = sizeof(server_addr);
  printf("Looking for clients...\n");


  listen(client, 1);



  int clientCount = 1;
  server = accept(client,(struct sockaddr *)&server_addr,&size);


  if (server < 0)
  	printf("Error on accepting\n");



	FILE* receivedFile = fopen("received.txt", "w");
	FILE * decodedFile = fopen ("decoded.txt", "w");
	int st;
	memset(buffer, 0, bufsize * sizeof(char));
 	if(st = read(server, buffer, bufsize) > 0){
		fwrite(buffer, sizeof(char),strlen(buffer), receivedFile);
		printf("entro \n");
		printf("recibi %s \n", buffer);
	}

	char* encoded = (char*)malloc(bufsize);
	char* decoded ;

	fclose(receivedFile);
	receivedFile = fopen("received.txt", "r");

	while(1){
		cont = fread(encoded,sizeof(char),bufsize,receivedFile);
		if (cont == 0) break;
		//Base64Decode(encodedBase64,&prueba,&test);
		size_t test;
		printf("encoded es: %s \n",encoded);
		cont = Base64Decode(encoded, &decoded, &test);
		printf("Llega %s \n", decoded);
		fwrite(decoded,sizeof(char),cont,decodedFile);
	}

	free (encoded);
	free(decoded);
	fclose(receivedFile);
	fclose(decodedFile);

  char * bufferino = "ACK";
  send(server,bufferino,bufsize,0);

  close(client);
  return 0;
}
