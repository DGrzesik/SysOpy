#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Wrong number of arguments!");
        return -1;
    }

    else if (strcmp(argv[1], "ignore") == 0){
        printf("\n------------ TESTING \"ignore\" ------------\n");
        signal(SIGUSR1, SIG_IGN);
        printf("Raising signal:\n");
        raise(SIGUSR1);
        execl("./child", "./child", argv[1], NULL);
        printf("If this message appeared, execl failure!\n");
    }
    else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0){
        if (strcmp(argv[1], "mask") == 0)
            printf("\n------------ TESTING \"mask\" ------------\n");
        else
            printf("\n------------ TESTING \"pending\" ------------\n");

        sigset_t new_mask;
        sigset_t old_mask;
        sigemptyset(&new_mask);
        sigaddset(&new_mask, SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0)
            perror("Signal blocking failed");

        printf("Raising signal:\n");
        raise(SIGUSR1);

        sigismember(&new_mask, SIGUSR1) ? printf("Signal pending\n") : printf("Signal  NOT pending\n");

        execl("./child", "./child", argv[1], NULL);
        printf("If this message appeared, execl failure!\n");
    }

    return 0;
}