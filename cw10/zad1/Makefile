CC = gcc
CFLAGS = -Wall -g

client:
	$(CC) $(FLAGS) client.c -o client -lpthread -lm

server:
	$(CC) $(FLAGS) server.c -o server -lpthread -lm

clean:
	rm -f server client socket