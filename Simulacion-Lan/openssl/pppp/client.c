#include <assert.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>


int Base64Encode(const unsigned char* buffer, size_t length, char** b64text) { //Encodes a binary safe base 64 string

  BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*b64text=(*bufferPtr).data;

	return (0); //success
}



size_t calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}



int main(){

  int client;
  int portNum = 1500; // NOTE that the port number is same for both client and server
  int bufsize = 512;
  char buffer[bufsize];
  char* ip = "127.0.0.1";
  char * text = "hola";
  char * encodedBase64;
  int id, st, cont;
  long sent, psize;
  int SIZE = 512;
  char* input = (char*)malloc(SIZE);
  char* encoded = (char*)malloc(SIZE);

  id = open("file to transfer", O_RDONLY );
  if ( -1 == id ) {
    printf( "File not found %s\n", "file to transfer" );
    return 1;
  }

  FILE* outputFile = fopen("encodedText.txt", "w");
  FILE * inputFile = fopen("input.txt", "r");
  while(1){
    cont = fread(input, sizeof(char),SIZE, inputFile);
    if (cont == 0) break;
    cont = Base64Encode(input,sizeof(char), &encodedBase64);
    fwrite(encoded, sizeof(char),cont, outputFile);
  }
  Base64Encode(text, strlen(text),&encodedBase64);
  fwrite(encoded, sizeof(char),cont, outputFile);

  fclose(inputFile);
  fclose(outputFile);

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
  memset(buffer, 0, bufsize * sizeof(char));
  while( st = read(id,buffer,SIZE)){
    write(client,buffer,SIZE);
    sent++;
  }


  recv(client, buffer, bufsize, 0);
  printf("recibi:  %s \n", buffer);

  close(client);
  close(id);
  return 0;
}
