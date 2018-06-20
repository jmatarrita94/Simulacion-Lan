#include <bitset>
#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <stdexcept>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <functional>
#include <openssl/evp.h>
#include <openssl/rand.h>

static const unsigned int KEY_SIZE = 32;
static const unsigned int BLOCK_SIZE = 16;

template <typename T>
struct zallocator
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    pointer address (reference v) const {return &v;}
    const_pointer address (const_reference v) const {return &v;}

    pointer allocate (size_type n, const void* hint = 0) {
        if (n > std::numeric_limits<size_type>::max() / sizeof(T))
            throw std::bad_alloc();
        return static_cast<pointer> (::operator new (n * sizeof (value_type)));
    }

    void deallocate(pointer p, size_type n) {
        OPENSSL_cleanse(p, n*sizeof(T));
        ::operator delete(p);
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof (T);
    }

    template<typename U>
    struct rebind
    {
        typedef zallocator<U> other;
    };

    void construct (pointer ptr, const T& val) {
        new (static_cast<T*>(ptr) ) T (val);
    }

    void destroy(pointer ptr) {
        static_cast<T*>(ptr)->~T();
    }

#if __cpluplus >= 201103L
    template<typename U, typename... Args>
    void construct (U* ptr, Args&&  ... args) {
        ::new (static_cast<void*> (ptr) ) U (std::forward<Args> (args)...);
    }

    template<typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }
#endif
};

typedef unsigned char byte;
typedef std::basic_string<char, std::char_traits<char>, zallocator<char> > secure_string;
using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

void gen_params(byte key[KEY_SIZE], byte iv[BLOCK_SIZE]);
void aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const secure_string& ptext, secure_string& ctext);
void aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const secure_string& ctext, secure_string& rtext);

using namespace std;

// g++ -Wall -std=c++11 evp-encrypt.cxx -o evp-encrypt.exe -lcrypto
int main(int argc, char* argv[])
{

    int client;
    int portNum = 1500; // NOTE that the port number is same for both client and server
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    if (client < 0)
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket client has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);


    // Load the necessary cipher
    EVP_add_cipher(EVP_aes_256_cbc());

    // plaintext, ciphertext, recovered text
    secure_string ptext = "Now is the time for all good men to come to the aide of their country";
    secure_string ctext, rtext;

    cout <<ptext<<endl;

    byte key[KEY_SIZE], iv[BLOCK_SIZE];
    gen_params(key, iv);

    aes_encrypt(key, iv, ptext, ctext);

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;

    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go...";

    cout << "\n\n=> Enter # to end the connection\n" << endl;
    size_t len = sizeof(ctext);
    char * c ;
    string i ;
    string arr [bufsize];
    int n = 0;

    cout<<ctext<<endl;
/*
    while (c != '\0'){
      c = ctext.at(i);
      arr[i] = c;
      i++;
      cout<<c;
    }

    cout << endl;

    for ( int j = 0; j < i; j++){
      cout<<arr[j];

    }*/

  for (std::size_t j = 0; j < ctext.size(); ++j)
  {
      i = bitset<8>(ctext.c_str()[j]).to_string();
      arr[j] = i;
    //  c += arr[j];
    //  cout << "i es: "<<i<<endl;


      n++;
  }

    cout<<endl;

    int t = 0;

    //stringstream sstream(i);
    string output;
    string output2;

    for ( int a = 0; a < n; a++){
      //i = arr[a];
      //output2 += decode(arr[a]);
      stringstream sstream(arr[a]);
        while (sstream.good()){
        bitset<8> bits;
        sstream >> bits;
        char b = char(bits.to_ulong());
        //cout << "b es: "<<b<<endl;
        output += b;
      }
    }

    cout<<endl;
    cout<<output<<endl;
    //<<ctext<<endl;
    ctext = output;

    cout<<"Sali del while"<<endl;

    //send(client,i.c_str(),len,0);
    //recv(client, buffer, bufsize, 0);

    aes_decrypt(key, iv, ctext, rtext);

    OPENSSL_cleanse(key, KEY_SIZE);
    OPENSSL_cleanse(iv, BLOCK_SIZE);

    std::cout << "Original message:\n" << ptext << std::endl;
    std::cout << "Recovered message:\n" << rtext << std::endl;

    return 0;
}

void gen_params(byte key[KEY_SIZE], byte iv[BLOCK_SIZE])
{
    int rc = RAND_bytes(key, KEY_SIZE);
    if (rc != 1)
      throw std::runtime_error("RAND_bytes key failed");

    rc = RAND_bytes(iv, BLOCK_SIZE);
    if (rc != 1)
      throw std::runtime_error("RAND_bytes for iv failed");
}

void aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const secure_string& ptext, secure_string& ctext)
{
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptInit_ex failed");

    // Recovered text expands upto BLOCK_SIZE
    ctext.resize(ptext.size()+BLOCK_SIZE);
    int out_len1 = (int)ctext.size();

    rc = EVP_EncryptUpdate(ctx.get(), (byte*)&ctext[0], &out_len1, (const byte*)&ptext[0], (int)ptext.size());
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptUpdate failed");

    int out_len2 = (int)ctext.size() - out_len1;
    rc = EVP_EncryptFinal_ex(ctx.get(), (byte*)&ctext[0]+out_len1, &out_len2);
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptFinal_ex failed");

    // Set cipher text size now that we know it
    ctext.resize(out_len1 + out_len2);
}

void aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const secure_string& ctext, secure_string& rtext)
{
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
    if (rc != 1)
      throw std::runtime_error("EVP_DecryptInit_ex failed");

    // Recovered text contracts upto BLOCK_SIZE
    rtext.resize(ctext.size());
    int out_len1 = (int)rtext.size();

    rc = EVP_DecryptUpdate(ctx.get(), (byte*)&rtext[0], &out_len1, (const byte*)&ctext[0], (int)ctext.size());
    if (rc != 1)
      throw std::runtime_error("EVP_DecryptUpdate failed");

    int out_len2 = (int)rtext.size() - out_len1;
    rc = EVP_DecryptFinal_ex(ctx.get(), (byte*)&rtext[0]+out_len1, &out_len2);
    if (rc != 1)
      throw std::runtime_error("EVP_DecryptFinal_ex failed");

    // Set recovered text size now that we know it
    rtext.resize(out_len1 + out_len2);
}
