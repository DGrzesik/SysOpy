test:
	rm -rf pomiar_zad_2.txt
	@echo "---TEST FUNKCJI SYSTEMOWYCH---\n" >> pomiar_zad_2.txt
	make --no-print-directory fire_s_f >> pomiar_zad_2.txt
	@echo "\n---TEST FUNKCJI BIBLIOTEKI C---\n" >> pomiar_zad_2.txt
	make --no-print-directory fire_c_f >> pomiar_zad_2.txt
fire_c_f:
	@gcc mainf.c -c
	@gcc mainf.o -o mainf
	@echo "\n---TEST 1: 'c' in source.txt ---\n" >> pomiar_zad_2.txt
	@./mainf c source.txt
	@echo "\n---TEST 2: 'b' in source2.txt---\n" >> pomiar_zad_2.txt
	@./mainf b source2.txt
	@echo "\n---TEST 3: 'a' in source3.txt---\n" >> pomiar_zad_2.txt
	@./mainf a source3.txt
fire_s_f:
	@gcc main.c -c
	@gcc main.o -o main
	@echo "\n---TEST 1: 'c' in source.txt---\n" >> pomiar_zad_2.txt
	@./main c source.txt
	@echo "\n---TEST 2: 'b' in source2.txt---\n" >> pomiar_zad_2.txt
	@./main b source2.txt
	@echo "\n---TEST 3: 'a' in source3.txt---\n" >> pomiar_zad_2.txt
	@./main a source3.txt
clean:
	rm -rf *.o main mainf