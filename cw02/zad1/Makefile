test:
	rm -rf pomiar_zad_1.txt
	@echo "---TEST FUNKCJI SYSTEMOWYCH---\n" >> pomiar_zad_1.txt
	make --no-print-directory copy >> pomiar_zad_1.txt
	@echo "\n---TEST FUNKCJI BIBLIOTEKI C---\n" >> pomiar_zad_1.txt
	make --no-print-directory copyf >> pomiar_zad_1.txt
copyf:
	@gcc mainf.c -c
	@gcc mainf.o -o mainf
	@echo "\n---TEST 1---\n" >> pomiar_zad_1.txt
	@./mainf from.txt to_f.txt
	@echo "\n---TEST 2---\n" >> pomiar_zad_1.txt
	@./mainf from2.txt to2_f.txt
copy:
	@gcc main.c -c
	@gcc main.o -o main
	@echo "\n---TEST 1---\n" >> pomiar_zad_1.txt
	@./main from.txt to.txt
	@echo "\n---TEST 2---\n" >> pomiar_zad_1.txt
	@./main from2.txt to2.txt
clean:
	rm -rf *.o main mainf