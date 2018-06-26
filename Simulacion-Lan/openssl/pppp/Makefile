CC=gcc
ARGS = -lssl -lcrypto

all : Client Server

Client:  client.c
		$(CC) client.c -o client $(ARGS)
Server: server.c
		$(CC) server.c -o server $(ARGS)
