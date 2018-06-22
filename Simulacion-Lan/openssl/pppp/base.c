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
//#include <iostream>

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

//using namespace std;

int main() {
  int client;
  int portNum = 1500; // NOTE that the port number is same for both client and server
//  bool isExit = false;
  int bufsize = 1024;
  char buffer[bufsize];
  char* ip = "127.0.0.1";

  struct sockaddr_in server_addr;

  client = socket(AF_INET, SOCK_STREAM, 0);

  /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
  /* --------------- socket() function ------------------*/

  if (client < 0)
  {
      printf("Error on socket \n");
      exit(1);
  }

  /*
      The socket() function creates a new socket.
      It takes 3 arguments,
          a. AF_INET: address domain of the socket.
          b. SOCK_STREAM: Type of socket. a stream socket in
          which characters are read in a continuous stream (TCP)
          c. Third is a protocol argument: should always be 0. The
          OS will choose the most appropiate protocol.
          This will return a small integer and is used for all
          references to this socket. If the socket call fails,
          it returns -1.
  */

printf("Socket created\n");

  /*
      The variable serv_addr is a structure of sockaddr_in.
      sin_family contains a code for the address family.
      It should always be set to AF_INET.
      htons() converts the port number from host byte order
      to a port number in network byte order.
  */

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portNum);

  // this function returns returns 1 if the IP is valid
  // and 0 if invalid
  // inet_pton converts IP to packets
  // inet_ntoa converts back packets to IP
  //inet_pton(AF_INET, ip, &server_addr.sin_addr);

  /*if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
      cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl;*/


  /* ---------- CONNECTING THE SOCKET ---------- */
  /* ---------------- connect() ---------------- */

  if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
      printf("connection made to %d", portNum);

  /*
      The connect function is called by the client to
      establish a connection to the server. It takes
      three arguments, the socket file descriptor, the
      address of the host to which it wants to connect
      (including the port number), and the size of this
      address.
      This function returns 0 on success and -1
      if it fails.
      Note that the client needs to know the port number of
      the server but not its own port number.
  */




  // Once it reaches here, the client can send a message first.
  buffer: "Holi";
  send(client, buffer, bufsize, 0);;

  close(client);
  return 0;
}
