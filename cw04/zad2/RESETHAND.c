#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void handler_RESETHAND(int signum) {
    printf("Received signal (%d). PID: %d, PPID: %d\n", signum, getpid(), getppid());
}

void test_RESETHAND() {
    printf("\n\n\n---------------- TEST FOR SA_RESETHAND ----------------\n");
    struct sigaction act;
    act.sa_handler = handler_RESETHAND;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);
    printf("\n----   SA_RESETHAND flag is NOT set   ----\n\n");
    raise(SIGUSR1);
    printf("\n----   SA_RESETHAND flag is set   ----\n\n");
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    printf("As there is no handler anymore, another signal returns default code: 1\n");
    raise(SIGUSR1);
}


int main() {
    test_RESETHAND();
}