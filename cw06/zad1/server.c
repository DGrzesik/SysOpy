#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include "shared.h"

client clients_queues[MAX_CLIENTS];
int server_queue;
void handle_message(message *msg);
void stop_client(int client_id);

void init_client(message *msg){
    int q_id = msg->q_id;
    pid_t client_pid = msg->sender_pid;
    int id;

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active == false){
            id = i;
            break;
        }
        id = -1111;
    }
    if(id == -1111){
        printf("Can't create another client: too many clients!\n");
        exit(3);
    }

    client c = {
            .client_queue = q_id,
            .client_pid = client_pid,
            .active = true
    };
    message response = {
            .type = INIT,
            .sender_id = id
    };
    clients_queues[id] = c;
    printf("[SERVER] Client created! id: %d, qid: %d\n", id, q_id);
    send_message(q_id, &response);
}

void init_clients_queue(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        client c = {c.active = false};
        clients_queues[i] = c;
    }
}

void stop_server(){
    printf("[SERVER] Server now shutting down...\n");

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active){
            printf("[SERVER] Sending STOP to client id: %d, qid: %d\n", i, clients_queues[i].client_queue);
            kill(clients_queues[i].client_pid, SIGINT);
            stop_client(i);
        }
    }
    delete_queue(server_queue);
    printf("\n SERVER STOPPED \n");
}

void stop_client(int client_id){
    if(clients_queues[client_id].active){
        printf("[SERVER] Disconnecting client: %d from queue\n", client_id);
        message msg = {.type = STOP};
        send_message(clients_queues[client_id].client_queue, &msg);
        client c = {.active = false};
        clients_queues[client_id] = c;
    }
}

void list_clients(int client_id){
    printf("\nCLIENTS:\n");
    char c_list[MAX_LEN];
    char temp[32];
    c_list[0] = '\0';

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active){
            printf("Client id: %d, qid: %d\n", i, clients_queues[client_id].client_queue);
            sprintf(temp, "%d\n", i);
            strcat(c_list, temp);
        }
    }
    printf("\n END OF CLIENT LIST\n");

    message msg = {
            .type = LIST
    };

    strcpy(msg.body, c_list);
    send_message(clients_queues[client_id].client_queue, &msg);
}

void send_to_one(int sender_id, int receiver_id, char* text) {
    if (clients_queues[receiver_id].active == false) {
        printf("[SERVER] Client id: %d doesn't exist\n", receiver_id);
        exit(11);
    }
    message msg = {
            .type = ONE,
            .sender_id = sender_id,
            .send_time = time(NULL)
    };
    strcpy(msg.body, text);
    send_message(clients_queues[receiver_id].client_queue, &msg);
}

void send_to_all(int sender_id, char* text){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active && i != sender_id){
            send_to_one(sender_id, i, text);
        }
    }
}

void save_msg_to_file(message *msg){
    FILE *file = fopen("server_messages.txt", "a");
    if(!file){
        printf("error while opening file\n");
        exit(20);
    }

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    fprintf(file, "date: %d-%02d-%02d\ntime: %02d:%02d:%02d\nclient_id: %d\nmessage_type: %ld\nmessage_body:\n'%s'\n\n",
            local_time->tm_year + 1900,
            local_time->tm_mon + 1,
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec,
            msg->sender_id,
            msg->type,
            msg->body
    );
    fclose(file);
}

void handle_msg(message* msg){
    save_msg_to_file(msg);

    switch (msg->type) {
        case STOP:
            printf("[SERVER] STOP order received\n");
            stop_client(msg->sender_id);
            break;
        case LIST:
            printf("[SERVER] LIST order received\n");
            list_clients(msg->sender_id);
            break;
        case INIT:
            printf("[SERVER] INIT order received\n");
            init_client(msg);
            break;
        case ALL:
            printf("[SERVER] 2ALL order received\n");
            send_to_all(msg->sender_id, msg->body);
            break;
        case ONE:
            printf("[SERVER] 2ONE order received\n");
            send_to_one(msg->sender_id, msg->receiver_id, msg->body);
            break;
        default:
            printf("[SERVER] Wrong message!\n");
            exit(6);
    }
}

void handle_sigint(){
    printf("[SERVER] I received signal: SIGINT\n");
    exit(0);
}

int main(){
    printf("\n[SERVER] Server now running...\n");

    atexit(stop_server);

    key_t key = ftok(home_path(), ID);

    if(key == -1){
        printf("Key generate error\n");
        return 1;
    }

    server_queue = msgget(key, IPC_EXCL | IPC_CREAT | 0666);

    if(server_queue == -1){

        if (errno == EEXIST){
            printf("Queue already exist\n");
            server_queue = msgget(key,  0666);
        }else{
            printf("Server queue create error\n");
            return 2;
        }

    }

    signal(SIGINT, handle_sigint);
    printf("Server queue id: %d\n", server_queue);
    init_clients_queue();

    message msg;
    while(true){
        int msgsz = MSG_SIZE;
        if(msgrcv(server_queue, &msg, msgsz, -INIT-1, 0) == -1){
            printf("Receive msg error");
            return 4;
        }
        handle_msg(&msg);
    }

    return 0;
}