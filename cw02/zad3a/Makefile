load:
	gcc main.c -c
	gcc main.o -o main
clean:
	rm -rf *.o *.a main
test:
	rm -rf pomiar_zad_2.txt
	@echo "---TEST ---\n" >> pomiar_zad_2.txt
	make --no-print-directory fire_s_f >> pomiar_zad_2.txt
	@echo "\n---TEST FUNKCJI BIBLIOTEKI C---\n" >> pomiar_zad_2.txt
	make --no-print-directory fire_c_f >> pomiar_zad_2.txt