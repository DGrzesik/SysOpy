#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]) {

    printf("Entered child program!\n");

    if (strcmp(argv[1], "ignore") == 0){
        printf("Raising signal in child process:\n");
        raise(SIGUSR1);
    }

    else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0){
        if (strcmp(argv[1], "mask") == 0) {
            printf("Raising signal in child process:\n");
            raise(SIGUSR1);
        }
        sigset_t new_mask;
        sigpending(&new_mask);
        sigismember(&new_mask, SIGUSR1) ? printf("Signal pending in child process!\n") : printf("Signal NOT pending in child process!\n");
    }

    return 0;
}