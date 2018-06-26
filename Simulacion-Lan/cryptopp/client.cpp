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

string encodeB64(string input){
  string in(input);

  string encoded;

  CryptoPP::StringSource ss(
    in,
    true,
    new CryptoPP::Base64Encoder(
        new CryptoPP::StringSink(encoded),
        false
    )
  );

  return encoded;
}

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

int encodeFile(char* name){
  int cnt = 0;
  FILE * inputFile = fopen(name, "r");
  FILE * outputFile = fopen("encoded.txt", "w");
  string encoded;
  char * input =(char*)malloc(size);
  while (1){
    cnt = fread(input, sizeof(char), size, inputFile);
    cout<<"llego"<<endl;
    if (cnt == 0) break;
    encoded = encodeB64(input);
    cout<<encoded<<endl;
    fwrite(encoded.c_str(), sizeof(char),strlen(encoded.c_str()), outputFile);
  }

  fclose(inputFile);
  fclose(outputFile);
  return 0;
}

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

  int client;
  int portNum = 1500; // NOTE that the port number is same for both client and server
  int bufsize = 512;
  char buffer[bufsize];
  char* ip = "127.0.0.1";
  int id;
  long sent, st;
  encodeFile("a.txt");
  decodeFile("encoded.txt");

  struct sockaddr_in server_addr;

  client = socket(AF_INET, SOCK_STREAM, 0);

  if (client < 0){
    printf("Error establishing socket \n");
    exit(1);
  }


  printf("Socket client has been created \n");


  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portNum);


  if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
    printf("Connection to the server port number : %d \n", portNum);


  id = open("encodedText.txt", O_RDONLY );
  if ( -1 == id ) {
    printf( "File not found %s\n", "file to transfer" );
    return 1;
  }

  sent = 0;
  int it = 0;
  memset(buffer, 0, bufsize * sizeof(char));
  while( st = read(id,buffer,size)){
    write(client,buffer,strlen(buffer + (size * it ) % 512));
    sent++;
  }


  recv(client, buffer, bufsize, 0);
  printf("recibi:  %s \n", buffer);

  close(client);
  close(id);
  return 0;

}
