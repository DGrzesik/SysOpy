#ifndef SHARED_H
#define SHARED_H

#include <stdlib.h>
#include <pwd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define ID 'R'
#define OVEN_ID 'O'
#define TABLE_ID 'T'

#define OVEN_PATH "oven"
#define TABLE_PATH "table"

#define OVEN_SIZE 5
#define TABLE_SIZE 5

#define OVEN_SEM 0
#define TABLE_SEM 1
#define FULL_OVEN_SEM 2
#define FULL_TABLE_SEM 3
#define EMPTY_TABLE_SEM 4

#define PREPARATION_TIME 2
#define BAKING_TIME 4
#define DELIVERY_TIME 4
#define RETURN_TIME 5

char* get_home_path(){
    char* path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}

typedef struct{
    int arr[OVEN_SIZE];
    int pizzas;
    int place_id;
    int take_out_id;
} oven;

typedef struct{
    int arr[TABLE_SIZE];
    int pizzas;
    int place_id;
    int take_out_id;
} table;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
} arg;


void lock_sem(int sem_id, int sem_num){
    struct sembuf s = {.sem_num = sem_num, .sem_op = -1};
    if (semop(sem_id, &s, 1) == -1){
        printf("Error while locking semaphore!\n");
        exit(1);
    }
}

void unlock_sem(int sem_id, int sem_num) {
    struct sembuf s = {.sem_num = sem_num, .sem_op = 1};
    if (semop(sem_id, &s, 1) == -1) {
        printf("Error while unlocking semaphore!\n");
        exit(1);
    }
}

int get_semaphore_set_id(){
    key_t key = ftok(get_home_path(), ID);
    int sem_id;
    if((sem_id = semget(key, 5,0)) == -1){
        printf("Error while connecting to semaphore set!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return sem_id;
}

int get_oven_shared_memory_id(){
    key_t key_o = ftok(OVEN_PATH, OVEN_ID);
    int shm_oven_id;
    if ((shm_oven_id = shmget(key_o, sizeof(oven), 0)) == -1){
        printf("Error while getting oven id!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return shm_oven_id;
}


int get_table_shared_memory_id(){
    key_t key_t = ftok(TABLE_PATH, TABLE_ID);
    int shm_table_id;
    if ((shm_table_id = shmget(key_t, sizeof(table), 0)) == -1){
        printf("Error while getting table id!\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return shm_table_id;
}


char* get_current_time(){
    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
    int millisec = cur_time.tv_usec / 1000;

    char* buffer = calloc(80, sizeof(char));
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&cur_time.tv_sec));

    char* current_time = calloc(84, sizeof(char));
    sprintf(current_time, "%s:%03d", buffer, millisec);
    return current_time;
}

#endif