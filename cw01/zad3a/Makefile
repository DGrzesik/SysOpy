CC = gcc
CFLAGS = -Wall -g

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o

shared:
	$(CC) -fPIC -c mylibrary.c
	$(CC) -shared mylibrary.o -o libmylibrary.so
test_all:
	rm -rf report3a.txt
	make test_static
	make test_shared
	make test_dynamic
clean:
	rm -rf *.o *.a *.so main
test_static: static
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main
	@echo "---------------STATIC TEST---------------" >> results3a.txt
	make  --no-print-directory small >> results3a.txt
	make  --no-print-directory medium >> results3a.txt
	make  --no-print-directory large >> results3a.txt
test_shared: shared
	$(CC) -c main.c
	$(CC) main.o  -L. -lmylibrary -Wl,-rpath=. -o main
	@echo "---------------TEST FOR SHARED---------------" >> results3a.txt
	make  --no-print-directory small >> results3a.txt
	make  --no-print-directory medium >> results3a.txt
	make  --no-print-directory large >> results3a.txt
test_dynamic:
	$(CC) -fPIC -c mylibrary.c
	$(CC) -shared mylibrary.o -o libmylibrary.so
	$(CC) main.c -o main -ldl -D DYN_LIB
	@echo "---------------TEST FOR DYNAMIC---------------" >> results3a.txt
	make  --no-print-directory small >> results3a.txt
	make  --no-print-directory medium >> results3a.txt
	make  --no-print-directory large >> results3a.txt
small:
	@echo "\nSMALL TESTS 3\n" >> results3a.txt
	@./main create_table 3 wc_files small1.txt small2.txt small3.txt \
 	remove_block 0 remove_block 1 remove_block 2
	@echo "\nSMALL TESTS 6\n" >> results3a.txt
	@./main create_table 6 wc_files small1.txt small2.txt small3.txt \
	small1.txt small2.txt small3.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5
	@echo "\nSMALL TESTS 10\n" >> results3a.txt
	@./main create_table 10 wc_files small1.txt small2.txt small3.txt \
	small1.txt small2.txt small3.txt small1.txt small2.txt small3.txt \
	small1.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9
medium:
	@echo "\nMEDIUM TESTS 3\n" >> results3a.txt
	@./main create_table 3 wc_files medium1.txt medium2.txt medium3.txt \
	remove_block 0 remove_block 1 remove_block 2
	@echo "\nMEDIUM TESTS 6\n" >> results3a.txt
	@./main create_table 6 wc_files medium1.txt medium2.txt medium3.txt \
	medium1.txt medium2.txt medium3.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5
	@echo "\nMEDIUM TESTS 10\n" >> results3a.txt
	@./main create_table 10 wc_files medium1.txt medium2.txt medium3.txt \
	medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium3.txt \
	medium1.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9
large:
	@echo "\n LARGE TESTS 3\n" >> results3a.txt
	@./main create_table 3 wc_files large1.txt large2.txt large3.txt remove_block 0 remove_block 1 remove_block 2
	@echo "\n LARGE TESTS 6\n" >> results3a.txt
	@./main create_table 6 wc_files large1.txt large2.txt large3.txt \
	large1.txt large2.txt large3.txt remove_block 0 remove_block 1 remove_block 2 \
	remove_block 3 remove_block 4 remove_block 5
	@echo "\n LARGE TESTS 10\n" >> results3a.txt
	@./main create_table 10 wc_files large1.txt large2.txt large3.txt \
	large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt \
	large1.txt remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9