CC = gcc
CFLAGS = -Wall -g

all: compile
compile:
	$(CC) $(FLAGS) server.c -o server
	$(CC) $(FLAGS) client.c -o client

clean:
	rm -f server client server_messages.txt