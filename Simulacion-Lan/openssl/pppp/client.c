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

/*Calculates the length of a char * for padding*/
size_t calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}

/**Decodes a char * messages that es encoded with openssl base64 encode algorythm*/
int Base64Decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string

	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	printf("Llega  \n");
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	printf("Llega  \n");
	(*buffer)[decodeLen] = '\0';
	printf("Llega  \n");
	bio = BIO_new_mem_buf(b64message, -1);
	printf("Llega  \n");
	b64 = BIO_new(BIO_f_base64());
	printf("Llega  \n");
	bio = BIO_push(b64, bio);
	printf("Llega  \n");

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	printf("Llega  \n");
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong

	printf("Llega A \n");
	BIO_free_all(bio);
	return (0); //success
}

/**Encodes a char * messages with openssl base64 encode algorythm*/
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
  char * prueba;


  size_t test;
 	/*Opens file that are received to encode*/
  FILE* outputFile = fopen("encodedText.txt", "w");
  FILE * inputFile = fopen("input.txt", "r");

	/*This loops makes sure that the file that is to be send to the server gets encoded with base64 openssl algorythm*/
  while(1){
    printf("Llego al while \n");
    cont = fread(input, sizeof(char),SIZE, inputFile);
    if (cont == 0) break;
    cont = Base64Encode(input,strlen(input), &encodedBase64);
		printf("Base64 es %s \n",encodedBase64);
    Base64Decode(encodedBase64,&prueba,&test);
    printf("desenmaaaa %s \n", prueba);
    printf("el buffer en Base64 es  y cont es  %s %d \n", encodedBase64, cont);
    fwrite(encodedBase64, sizeof(char),strlen(encodedBase64), outputFile);
  }

  fclose(inputFile);
  fclose(outputFile);

	/*The logic of creating a socket, connecting it with the server and making sure everything is okay starts here*/
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

	/*End*/

	/*Opens the encoded file to be send to the server side*/
  id = open("encodedText.txt", O_RDONLY );
  if ( -1 == id ) {
    printf( "File not found %s\n", "file to transfer" );
    return 1;
  }

  sent = 0;
  int it = 0;
  memset(buffer, 0, bufsize * sizeof(char));
  while( st = read(id,buffer,SIZE)){
    write(client,buffer,strlen(buffer + (SIZE * it ) % 512));
    sent++;
  }

	/*receives confirmation**/
  recv(client, buffer, bufsize, 0);
  printf("recibi:  %s \n", buffer);

  close(client);
  close(id);
  return 0;
}
