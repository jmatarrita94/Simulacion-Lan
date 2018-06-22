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

size_t calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return (0); //success
}

int main()
{

    int client, server;
    int portNum = 1500;
    int bufsize = 1024;
    char buffer[bufsize];

    struct sockaddr_in server_addr;
    socklen_t size;


    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        printf("Error establishing socket...\n");
        exit(1);
    }


    printf("Socket Server has been created \n");


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);



    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
    {
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

    recv(server, buffer, 7, 0);
    printf("recibi %s \n", buffer);
    size_t test;
    char * defecodeOutput;
    Base64Decode(buffer, &defecodeOutput, &test);

    char * bufferino = "Hola ravioli";
    send(server,bufferino,bufsize,0);

    close(client);
    return 0;
}
