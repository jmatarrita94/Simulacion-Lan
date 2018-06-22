#include <stdio.h>
#include <string.h>
//Decodes Base64
#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdint.h>
#include <assert.h>
#include <openssl/buffer.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>


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


int main()
{

    int client;
    int portNum = 1500; // NOTE that the port number is same for both client and server
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
    char * text = "hola";
    char * encodedBase64;

    Base64Encode(text, strlen(text), &encodedBase64);

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        printf("Error establishing socket \n");
        exit(1);
    }


    printf("Socket client has been created \n");


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);


    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        printf("Connection to the server port number : %d \n", portNum);


    char * bufferino = "hola ravioli";
    //printf("mande: %s \n", encodedBase64);
    for (int i = 0; i < strlen(encodedBase64); i++){
      printf("mande: %c \n", encodedBase64[i]);
      printf("es i %d\n",i );
    }
    send(client,encodedBase64,sizeof(encodedBase64),0);
    recv(client, buffer, bufsize, 0);
    printf("recibi:  %s \n", buffer);

    close(client);
    return 0;
}
