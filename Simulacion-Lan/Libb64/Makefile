CC = g++
ARGS = -l rt -lcrypto -lb64 -std=c++11

all: Client Server

Client : Cliente.cpp
		$(CC) Cliente.cpp -o client $(ARGS)

Server : server.cpp
		$(CC) server.cpp -o server $(ARGS)

