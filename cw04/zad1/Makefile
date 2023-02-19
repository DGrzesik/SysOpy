CC = gcc
CFLAGS = -Wall -g

all:
	@echo "\n---- FORK TEST ----\n"
	@make --no-print-directory test_fork
	@echo "\n---- EXEC TEST ----\n"
	@make --no-print-directory test_exec

test_fork: compile_fork
	@./fork ignore
	@./fork handler
	@./fork mask
	@./fork pending

compile_fork:
	@$(CC) $(FLAGS) fork.c -o fork

test_exec: compile_exec
	@./exec ignore
	@./exec mask
	@./exec pending

compile_exec:
	@$(CC) $(FLAGS) exec.c -o exec
	@$(CC) $(FLAGS) child.c -o child

clean:
	rm -rf fork exec child