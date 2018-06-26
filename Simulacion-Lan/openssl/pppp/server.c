#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>


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
	printf("Llega as \n");
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong

	printf("Llega A \n");
	BIO_free_all(bio);
	return (0); //success
}

int main()
{

		/*The logic of creating a socket, connecting it with the server and making sure everything is okay starts here*/
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

	/*End*/



	FILE* receivedFile = fopen("received.txt", "w");
	FILE * decodedFile = fopen ("decoded.txt", "w");

	/**/
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
