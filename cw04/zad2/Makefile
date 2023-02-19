CFLAGS = -Wall -g
all: run

compile:
	@gcc $(CFLAGS) RESETHAND.c -o RESETHAND
	@gcc $(CFLAGS) SIGINFO_NOCLDSTOP.c -o SIGINFO_NOCLDSTOP

run: clean compile
	@./SIGINFO_NOCLDSTOP $(ARGS)
	@./RESETHAND $(ARGS)

clean:
	@rm -rf RESETHAND SIGINFO_NOCLDSTOP