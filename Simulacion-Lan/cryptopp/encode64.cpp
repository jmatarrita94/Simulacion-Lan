#include <crypto++/cryptlib.h>
#include <cryptopp/base64.h>
#include <iostream>
#include <string.h>

using namespace std;

int mai(){

byte decoded[] = { 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
string encoded;

Base64Encoder encoder;
encoder.Put(decoded, sizeof(decoded));
encoder.MessageEnd();

word64 size = encoder.MaxRetrievable();
if(size)
{
    encoded.resize(size);
    encoder.Get((byte*)&encoded[0], encoded.size());
}

cout << encoded << endl;
}
