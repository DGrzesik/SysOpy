CC = gcc
CFLAGS = -Wall -g

all:test

test:compile
	./main 1 numbers images/totem.ascii.pgm negative/n1_totem.ascii.pgm
	./main 1 block images/totem.ascii.pgm negative/b1_totem.ascii.pgm
	./main 2 numbers images/totem.ascii.pgm negative/n2_totem.ascii.pgm
	./main 2 block images/totem.ascii.pgm negative/b2_totem.ascii.pgm
	./main 4 numbers images/totem.ascii.pgm negative/n4_totem.ascii.pgm
	./main 4 block images/totem.ascii.pgm negative/b4_totem.ascii.pgm
	./main 8 numbers images/totem.ascii.pgm negative/n8_totem.ascii.pgm
	./main 8 block images/totem.ascii.pgm negative/b8_totem.ascii.pgm
	./main 16 numbers images/totem.ascii.pgm negative/n16_totem.ascii.pgm
	./main 16 block images/totem.ascii.pgm negative/b16_totem.ascii.pgm
	./main 64 numbers images/totem.ascii.pgm negative/n64_totem.ascii.pgm
	./main 64 block images/totem.ascii.pgm negative/b64_totem.ascii.pgm
	./main 1 numbers images/dla.ascii.pgm negative/n1_dla.ascii.pgm
	./main 1 block images/dla.ascii.pgm negative/b1_dla.ascii.pgm
	./main 2 numbers images/dla.ascii.pgm negative/n2_dla.ascii.pgm
	./main 2 block images/dla.ascii.pgm negative/b2_dla.ascii.pgm
	./main 4 numbers images/dla.ascii.pgm negative/n4_dla.ascii.pgm
	./main 4 block images/dla.ascii.pgm negative/b4_dla.ascii.pgm
	./main 8 numbers images/dla.ascii.pgm negative/n8_dla.ascii.pgm
	./main 8 block images/dla.ascii.pgm negative/b8_dla.ascii.pgm
	./main 16 numbers images/dla.ascii.pgm negative/n16_dla.ascii.pgm
	./main 16 block images/dla.ascii.pgm negative/b16_dla.ascii.pgm
	./main 64 numbers images/dla.ascii.pgm negative/n64_dla.ascii.pgm
	./main 64 block images/dla.ascii.pgm negative/b64_dla.ascii.pgm
compile:
	@$(CC) $(FLAGS) main.c -o main -lpthread

clean:
	@rm -f main


