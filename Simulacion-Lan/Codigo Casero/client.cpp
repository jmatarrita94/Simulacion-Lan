#include "base64.cpp"
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

#define SIZE 512
using namespace std;


/*
	decodes a base64 encoded file using libb64 encode algorythm
*/
void encodeFile (FILE* inputFile, FILE* outputFile){

	int size = SIZE;
	char * input =  (char*)malloc(size);
	string encoded; /* ~4/3 x input */
	/* we need an encoder and decoder state */

	/* store the number of bytes encoded by a single call */
	int cnt = 0;

	/*---------- START ENCODING ----------*/
	/* initialise the encoder state */
	/* gather data from the input and send it to the output */
	while (1)
	{
		cnt = fread(input, sizeof(char), size, inputFile);
		if (cnt == 0) break;

		encoded = base64_encode(reinterpret_cast<const unsigned char*>(input), strlen(input));
		/* output the encoded bytes to the output file */
    cout<<"Es: "<<encoded<< " con tamano "<<strlen(encoded.c_str())<<endl;
		fwrite(encoded.c_str(), sizeof(char), strlen(encoded.c_str()), outputFile);
	}
	/* since we have reached the end of the input file, we know that
	   there is no more input data; finalise the encoding */
	encoded = base64_encode(reinterpret_cast<const unsigned char*>(input), strlen(input));
	/* write the last bytes to the output file */
	//fwrite(encoded.c_str(), sizeof(char), strlen(encoded.c_str()), outputFile);
	/*---------- STOP ENCODING  ----------*/


	free(input);

}



/*
	handles all the logic of the client side, creating the socket and connecting it to the server plus send the
	file to be send to the server to encryption before sending it. Also times how long it took the process to be done.
*/
int doClient (char * encodedFile){
	int client;
	int portNum = 1500; // NOTE that the port number is same for both client and server
	bool isExit = false;
	int bufsize = 512;
	long sent, psize;
	int id, st;
	double rate;
	char buffer[bufsize];
	char* ip = "127.0.0.1";
	Chrono ci, cf;

	psize = 128*1024*1024;

	struct sockaddr_in server_addr;

	 client = socket(AF_INET, SOCK_STREAM, 0);

	 if (client < 0)
	 {
			 cout << "\nError establishing socket..." << endl;
			 exit(1);
	 }


	 cout << "\n=> Socket client has been created..." << endl;


	 server_addr.sin_family = AF_INET;
	 server_addr.sin_port = htons(portNum);


	 if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0){
			 cout << "=> Connection to the server port number: " << portNum << endl;
		}

		id = open (encodedFile , O_RDONLY);
		if (id == -1){
			cout<<"File not found or invalid"<<endl;
			return -1;
		}

		ci.getTime();
		sent = 0;
		int it = 0;
		memset(buffer, 0, bufsize * sizeof(char));
		while(st = read (id, buffer, SIZE) ){
			cout<<"algo"<<endl;
			st = write(client,buffer, strlen(buffer + (SIZE * it) % 512));
			sent += st;
			cout<<"Enviado : "<<sent<<endl;
			if ((sent % psize) == 0){
				cout<<"algo"<<endl;
				cf.getTime();
				cf -= ci;
				rate = cf.getSecs() + (cf.getnSecs() / 1000000000);
				rate = (double)(sent/1024/1024)/rate;
			}
			cout<<"algo"<<endl;
		}

		recv(client, buffer, bufsize, 0);
		cf.getTime();	// Get the time now
		cf -= ci;		// Calculate the difference

		printf( "Time taken to transfer %ld bytes is: %ld sec., %ld ns\n", sent, cf.getSecs(), cf.getnSecs() );

		printf( "%s\n", buffer );		// Print the string

		rate = cf.getSecs() + (cf.getnSecs()/1000000000);
		rate = (double) (1 / 1024 /1024) / rate;
		printf( "Total transfer rate: %f MBps\n", rate );
		/*char * bufferino = "hola ravioli";
		send(client,bufferino,bufsize,0);
	 	recv(client, buffer, bufsize, 0);
	 	cout << "=> Connection confirmed, you are good to go...";
		*/

	 	cout << "\n\n=> Enter # to end the connection\n" << endl;

	 	cout << "\n=> Connection terminated.\nGoodbye...\n";

	 	close(client);
	 	return cf.getnSecs();


}




int main (){

  char * prueba = "aaa";
  string encode, decode;
  encode = base64_encode(reinterpret_cast<const unsigned char*>(prueba), strlen(prueba));
  decode = base64_decode(encode);

  cout<<"Esto es encode: "<<encode<<endl;
  cout<<"Asi es cuando se decodifica: "<<decode<<endl;

  FILE* inputFile = fopen("aaa.txt","r");
  FILE * encodedFile = fopen("encodedTest.text", "w");
  encodeFile(inputFile,encodedFile);

  fclose(inputFile);
  fclose(encodedFile);

  doClient("encodedTest.text");


}
