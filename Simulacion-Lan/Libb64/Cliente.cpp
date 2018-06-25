#include <b64/encode.h>
#include <b64/decode.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include "Chrono.cpp"
#define SIZE 512
using namespace std;

void encode (FILE* inputFile, FILE* outputFile){
	base64::encoder E;
	int size = SIZE;
	char * input =  (char*)malloc(size);
	char* encoded = (char*)malloc(2*size); /* ~4/3 x input */
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
		cnt = E.encode(input, sizeof(input), encoded);
		/* output the encoded bytes to the output file */
		fwrite(encoded, sizeof(char), cnt, outputFile);
	}
	/* since we have reached the end of the input file, we know that
	   there is no more input data; finalise the encoding */
	cnt = E.encode(input, sizeof(input), encoded);
	/* write the last bytes to the output file */
	fwrite(encoded, sizeof(char), cnt, outputFile);
	/*---------- STOP ENCODING  ----------*/

	free(encoded);
	free(input);

}



void decode(FILE* inputFile, FILE* outputFile)
{
	base64::decoder E;
	/* set up a destination buffer large enough to hold the decoded data */
	int size = SIZE;
	char* encoded = (char*)malloc(size);
	char* decoded = (char*)malloc(1*size); /* ~3/4 x encoded */
	/* we need an encoder and decoder state */
	/* store the number of bytes encoded by a single call */
	int cnt = 0;

	/* gather data from the input and send it to the output */
	while (1)
	{
		cnt = fread(encoded, sizeof(char), size, inputFile);
		if (cnt == 0) break;
		cnt = E.decode(encoded, sizeof(encoded), decoded);
		/* output the encoded bytes to the output file */
		fwrite(decoded, sizeof(char), cnt, outputFile);
	}
	/*---------- START DECODING  ----------*/

	free(encoded);
	free(decoded);
}



int client (char * encodedFile){
	int client;
	int portNum = 1500; // NOTE that the port number is same for both client and server
	bool isExit = false;
	int bufsize = 1024;
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
		memset(buffer, 0, bufsize * sizeof(char));
		while(st = read (id, buffer, SIZE) ){
			cout<<"algo"<<endl;
			st = write(client,buffer, SIZE);
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

		cf.getTime();	// Get the time now
		cf -= ci;		// Calculate the difference

		printf( "Time taken to transfer %ld bytes is: %ld sec., %ld ns\n", sent, cf.getSecs(), cf.getnSecs() );

		printf( "%s\n", buffer );		// Print the string

		rate = cf.getSecs() + (cf.getnSecs()/1000000000);
		rate = (double) (sent / 1024 /1024) / rate;
		printf( "Total transfer rate: %f MBps\n", rate );
		/*char * bufferino = "hola ravioli";
		send(client,bufferino,bufsize,0);
	 	recv(client, buffer, bufsize, 0);
	 	cout << "=> Connection confirmed, you are good to go...";
		*/

	 	cout << "\n\n=> Enter # to end the connection\n" << endl;

	 	cout << "\n=> Connection terminated.\nGoodbye...\n";

	 	close(client);
	 	return 0;


}



int main(){
	base64::encoder E;
	char* encoded = (char*)malloc(SIZE);
	E.encode("Hola Mundo",sizeof("Hola Mundo"),encoded);
	cout<<encoded<<endl;
	FILE* inputFile = fopen("aaa.txt","r");
	FILE * encodedFile = fopen("encodedTest.text", "w");
	encode(inputFile,encodedFile);

	fclose(inputFile);
	fclose(encodedFile);

	if  ( client("encodedTest.text")<0 ){
		cout<<"No se pudo mandar el mensaje"<<endl;
	}else{
		cout<<"Se mando el mensaje"<<endl;
	}

/*
	encodedFile = fopen("encodedTest.text", "r");
	FILE* dencodedFile = fopen("dencoded.txt", "w");

	decode(encodedFile, dencodedFile);

	fclose(encodedFile);
	fclose(dencodedFile);
	*/
	return 0;
	}
