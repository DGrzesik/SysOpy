#ifndef SHARED_H
#define SHARED_H


#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define MAX_MSG_LENGTH 256

typedef enum{
    EMPTY,
    O,
    X
} slot;

typedef struct{
    int move;
    slot slots[9];

} game_board;

typedef struct{
    char* name;
    int fd;
    bool available;
} client;

typedef enum{
    START,
    WAIT_FOR_OPPONENT,
    WAIT_FOR_MOVE,
    OPPONENT_MOVE,
    MOVE,
    QUIT
} state;

#endif