CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	$(CC) $(FLAGS) main.c -o main

run: compile
	./main commands.txt

clean:
	rm -f main