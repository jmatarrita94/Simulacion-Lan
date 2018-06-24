#include <b64/encode.h>
#include <b64/decode.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
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



int main(){
	base64::encoder E;
	char* encoded = (char*)malloc(SIZE);
	char* input = "Hola";
	int len = sizeof(input);
	E.encode(input,len ,encoded);
	cout<<encoded<<endl;
	return 0;
	}
