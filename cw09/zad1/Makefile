CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	$(CC) $(FLAGS) main.c -o main -lpthread


run: compile
	./main

clean:
	rm -f main