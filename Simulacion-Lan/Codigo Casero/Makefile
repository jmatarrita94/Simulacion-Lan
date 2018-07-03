CC = g++
ARGS = -l rt -std=c++11

all: Client Server

Client: client.cpp
		$(CC) client.cpp -o client $(ARGS)

Server: server.cpp
		$(CC) server.cpp -o server $(ARGS)
