load:clean
	@gcc main.c -c
	@gcc main.o -lm -o main
clean:
	@rm -rf *.o main *.txt
test:load
	@echo "---TEST DLA SZEROKOŚCI 0.0002 I 25 PROCESÓW---\n" >> raport2.txt
	make --no-print-directory test1 >> raport2.txt
	@rm -rf *.o main w*.txt
	@echo "---TEST DLA SZEROKOŚCI 0.0002 I 500 PROCESÓW---\n" >> raport2.txt
	make --no-print-directory test2 >> raport2.txt
	@rm -rf *.o main w*.txt
	@echo "---TEST DLA SZEROKOŚCI 0.0002 I 1000 PROCESÓW---\n" >> raport2.txt
	make --no-print-directory test3 >> raport2.txt
	@rm -rf *.o main w*.txt
	@echo "---TEST DLA SZEROKOŚCI 0.000002 I 25 PROCESÓW---\n" >> raport2.txt
	make --no-print-directory test4 >> raport2.txt
	@rm -rf *.o main w*.txt
	@echo "---TEST DLA SZEROKOŚCI 0.000002 I 500 PROCESÓW---\n" >> raport2.txt
	make --no-print-directory test5 >> raport2.txt
	@rm -rf *.o main w*.txt
	@echo "---TEST DLA SZEROKOŚCI 0.000002 I 1000 PROCESÓW---\n" >> raport2.txt
	make --no-print-directory test6 >> raport2.txt
	@rm -rf *.o main w*.txt
test1:
	@gcc main.c -c
	@gcc main.o -lm -o main
	@./main 0.0002 25
test2:
	@gcc main.c -c
	@gcc main.o -lm -o main
	@./main 0.0002 500
test3:
	@gcc main.c -c
	@gcc main.o -lm -o main
	@./main 0.0002 1000
test4:
	@gcc main.c -c
	@gcc main.o -lm -o main
	@./main 0.000002 25
test5:
	@gcc main.c -c
	@gcc main.o -lm -o main
	@./main 0.000002 500
test6:
	@gcc main.c -c
	@gcc main.o -lm -o main
	@./main 0.000002 1000