load:
	gcc main.c -c
	gcc main.o -o main
clean:
	rm -rf *.o *.a main
test: load
	./main . szuk 4
	@make --no-print-directory clean