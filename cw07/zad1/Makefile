CC = gcc
CFLAGS = -Wall -g

all:run

run:compile
	./main $(ARGS)

compile:
	@touch oven
	@touch table
	@$(CC) $(FLAGS) cook.c -o cook
	@$(CC) $(FLAGS) delivery_man.c -o delivery_man
	@$(CC) $(FLAGS) main.c -o main

clean:
	@rm -f main cook delivery_man oven table

