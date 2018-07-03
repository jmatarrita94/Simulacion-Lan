
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
#include "Chrono.cpp"

using namespace std;
using namespace CryptoPP;
int size = 512;



/*
  Encodes a string with cryptopp base64 algorythm

*/
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

/*
  Encodes a file with base64 cryptopp algorythm
*/
int encodeFile(char* name){
  int cnt = 0;
  FILE * inputFile = fopen(name, "r");
  FILE * outputFile = fopen("encoded.txt", "w");
  string encoded;
  int i = 0;
  char * input =(char*)malloc(size);
  while (1){
    cnt = fread(input, sizeof(char), size, inputFile);
    i++;
    cout<<"llego"<<endl;
    if (cnt == 0) break;
    encoded = encodeB64(input);
    cout<<encoded<<endl;
    fwrite(encoded.c_str(), sizeof(char),strlen(encoded.c_str()), outputFile);
  }
  cout<<i<<"es i"<<endl;
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
  long sent, st, psize;
  Chrono ci, cf;
  double rate;
  psize = 128*1024*1024;

  //encodes the file to be transfer
  encodeFile("a.txt");

    /*
    Creates a socket and makes sure that it works

  */
  struct sockaddr_in server_addr;

  client = socket(AF_INET, SOCK_STREAM, 0);

  if (client < 0){
    printf("Error establishing socket \n");
    exit(1);
  }


  printf("Socket client has been created \n");


  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portNum);

/***/

  /*conects the socket client to a server*/
  if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
    printf("Connection to the server port number : %d \n", portNum);


  //opens the file to be transfer to the server side
  id = open("encoded.txt", O_RDONLY );
  if ( -1 == id ) {
    printf( "File not found %s\n", "file to transfer" );
    return 1;
  }

  //sents the file to the server and takes times
  ci.getTime();
  sent = 0;
  int it = 0;
  int i = 0;
  memset(buffer, 0, bufsize * sizeof(char));
  while(st = read(id,buffer,size)){
    cout<<buffer<< "es i aajo"<<endl;
    //int satan = strlen(buffer + (size * it )) % 512;
    //printf("len: %d\n", satan);
    write(client,buffer, st);
    memset(buffer, 0, bufsize * sizeof(char));
    sent++;
    it++;
    if ((sent % psize) == 0){
      cout<<"algo"<<endl;
      cf.getTime();
      cf -= ci;
      rate = cf.getSecs() + (cf.getnSecs() / 1000000000);
      rate = (double)(sent/1024/1024)/rate;
    }
  }

  shutdown( client, SHUT_WR );

  //receives confirmation that it finished
  recv(client, buffer, bufsize, 0);
  printf("recibi:  %s \n", buffer);
  cf.getTime();	// Get the time now
  cf -= ci;		// Calculate the difference

  printf( "Time taken to transfer %ld bytes is: %ld sec., %ld ns\n", sent, cf.getSecs(), cf.getnSecs() );

  rate = cf.getSecs() + (cf.getnSecs()/1000000000);
  rate = (double) (sent / 1024 /1024) / rate;
  printf( "Total transfer rate: %f MBps\n", rate );

  close(client);
  close(id);
  return 0;

}
