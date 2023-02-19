#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler_SIGINT(int sig, siginfo_t *info, void *ucontext) {
    printf("\n\nReceived SIGINT signal (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("Info:\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    kill(info->si_pid, SIGINT);
}

void handler_SIGCHLD(int sig, siginfo_t *info, void *ucontext) {
    printf("\n\nReceived SIGCHLD signal (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("Info:\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    exit(0);
}


void handler_SIGINT_as_child(int sig, siginfo_t *info, void *ucontext) {
    printf("\n\nReceived SIGINT signal [as child] (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("Info:\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    exit(0);
}


void handler_NOCLDSTOP(int signum) {
    printf("Received signal SIGCHLD (%d). PID: %d, PPID: %d\n", signum, getpid(), getppid());
}

void test_SIGINFO() {
    printf("\n\n\n---------------- TEST FOR SA_SIGINFO ----------------\n");
    struct sigaction act_SIGINT;
    sigemptyset(&act_SIGINT.sa_mask);
    act_SIGINT.sa_sigaction = handler_SIGINT;
    act_SIGINT.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act_SIGINT, NULL);
    struct sigaction act_SIGCHLD;
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_sigaction = handler_SIGCHLD;
    act_SIGCHLD.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &act_SIGCHLD, NULL);


    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, (__sighandler_t) & handler_SIGINT_as_child);
        kill(getppid(), SIGINT);
        pause();

    } else {
        while (1) {}
    }
}


void test_NOCLDSTOP() {
    printf("\n\n\n---------------- TEST FOR SA_NOCLDSTOP ----------------\n");
    struct sigaction act_SIGCHLD;
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_handler = &handler_NOCLDSTOP;
    sigaction(SIGCHLD, &act_SIGCHLD, NULL);
    // fork and stop child process without flag
    printf("\n---- SA_NOCLDSTOP flag is NOT set ----\n\n");
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process: %d\n", getpid());
        while (1) {}
    } else {
        printf("Parent process: %d\n", getpid());
        printf("Child process stopped, SIGCHLD should be sent...\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }
    sleep(2);
    printf("\n----   SA_NOCLDSTOP flag is set   ----\n\n");
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_handler = &handler_NOCLDSTOP;
    act_SIGCHLD.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &act_SIGCHLD, NULL);
    pid = fork();
    if (pid == 0) {
        printf("Child process: %d\n", getpid());
        while (1) {}
    } else {
        printf("Parent process: %d\n", getpid());
        printf("Child process stopped, but SIGCHLD should NOT be sent...\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }
    sleep(2);
}


int main() {
    test_NOCLDSTOP();
    test_SIGINFO();
    return 0;
}
