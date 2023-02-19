CC = gcc
CFLAGS = -Wall -g

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o

clean:
	rm -rf *.o *.a *.so main
test_static: static
	rm -rf raport2.txt
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main
	@echo "TEST FOR STATIC" >> raport2.txt
	make  --no-print-directory small >> raport2.txt
	make  --no-print-directory medium >> raport2.txt
	make  --no-print-directory large >> raport2.txt
small:
	@echo "\nSMALL TESTS 3\n" >> raport2.txt
	@./main create_table 3 wc_files small1.txt small2.txt small3.txt \
 	remove_block 0 remove_block 1 remove_block 2
	@echo "\nSMALL TESTS 6\n" >> raport2.txt
	@./main create_table 6 wc_files small1.txt small2.txt small3.txt \
	small1.txt small2.txt small3.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5
	@echo "\nSMALL TESTS 10\n" >> raport2.txt
	@./main create_table 10 wc_files small1.txt small2.txt small3.txt \
	small1.txt small2.txt small3.txt small1.txt small2.txt small3.txt \
	small1.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9
medium:
	@echo "\nMEDIUM TESTS 3\n" >> raport2.txt
	@./main create_table 3 wc_files medium1.txt medium2.txt medium3.txt \
	remove_block 0 remove_block 1 remove_block 2
	@echo "\nMEDIUM TESTS 6\n" >> raport2.txt
	@./main create_table 6 wc_files medium1.txt medium2.txt medium3.txt \
	medium1.txt medium2.txt medium3.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5
	@echo "\nMEDIUM TESTS 3\n" >> raport2.txt
	@./main create_table 10 wc_files medium1.txt medium2.txt medium3.txt \
	medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium3.txt \
	medium1.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9
large:
	@echo "\n LARGE TESTS 3\n" >> raport2.txt
	@./main create_table 3 wc_files large1.txt large2.txt large3.txt remove_block 0 remove_block 1 remove_block 2
	@echo "\n LARGE TESTS 6\n" >> raport2.txt
	@./main create_table 6 wc_files large1.txt large2.txt large3.txt \
	large1.txt large2.txt large3.txt remove_block 0 remove_block 1 remove_block 2 \
	remove_block 3 remove_block 4 remove_block 5
	@echo "\n LARGE TESTS 10\n" >> raport2.txt
	@./main create_table 10 wc_files large1.txt large2.txt large3.txt \
	large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt \
	large1.txt remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9