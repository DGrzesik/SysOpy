#include "shared.h"

client *clients[MAX_CLIENTS] = {NULL};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int no_clients = 0;

int get_opponent(int id){
    if (id % 2 == 0){
        return id + 1;
    }
    else{
        return id - 1;
    }
}

int find_client(char *name){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int add_client(char *name, int fd){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0) {
            return -1;
        }
    }

    int id = -1;

    for (int i = 0; i < MAX_CLIENTS; i += 2){
        if (clients[i] != NULL && clients[i + 1] == NULL){
            id = i + 1;
            break;
        }
    }

    if (id == -1){
        for (int i = 0; i < MAX_CLIENTS; i++){
            if (clients[i] == NULL){
                id = i;
                break;
            }
        }
    }

    if (id != -1){
        client *client = calloc(1, sizeof(client));
        client->name = calloc(MAX_MSG_LENGTH, sizeof(char));
        strcpy(client->name, name);
        client->fd = fd;
        client->available = true;
        clients[id] = client;
        no_clients++;
    }

    return id;
}

void free_client(int index){
    free(clients[index]->name);
    free(clients[index]);
    clients[index] = NULL;
    no_clients--;

    int opponent = get_opponent(index);

    if (clients[get_opponent(index)] != NULL){
        printf("Removing opponent: %s\n", clients[opponent]->name);
        free(clients[opponent]->name);
        free(clients[opponent]);
        clients[opponent] = NULL;
        no_clients--;
    }
}

void remove_client(char *name){
    int id = -1;
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->name, name) == 0) {
            id = i;
        }
    }

    printf("Removing client: %s\n", name);
    free_client(id);
}


void delete_not_available_clients(){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] && ! clients[i]->available){
            printf("Client not available\n");
            remove_client(clients[i]->name);
        }
    }
}

void send_pings(){
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i]){
            send(clients[i]->fd, "ping; ", MAX_MSG_LENGTH, 0);
            clients[i]->available = false;
        }
    }
}

void* ping(){
    while (true){
        printf("PING\n");
        pthread_mutex_lock(&mutex);
        delete_not_available_clients();
        send_pings();
        pthread_mutex_unlock(&mutex);

        sleep(10);
    }
}

int set_unix_socket(char* socket_path){
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    int status;
    if (sock_fd  == -1){
        printf("Error while creating LOCAL socket!\n");
        exit(1);
    }
    else{
        printf("LOCAL socket created!\n");
    }

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, socket_path);

    unlink(socket_path);
    status=bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    if (status == -1){
        printf("Error while binding LOCAL socket!\n");
        exit(1);
    }
    status=listen(sock_fd, MAX_CLIENTS);
    if (status == -1){
        printf("Error in listen on LOCAL socket!\n");
        exit(1);
    }

    printf("LOCAL socket fd: %d\n", sock_fd);

    return sock_fd;
}

int set_inet_socket(char *port){
    int sock_fd = socket(AF_INET , SOCK_STREAM, 0);
    int status;
    if (sock_fd  == -1){
        printf("Error while creating INET socket!\n");
        exit(1);
    }
    else{
        printf("INET socket created!\n");
    }

    struct sockaddr_in sock_addr;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(atoi(port));

    status=bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    if (status == -1){
        printf("Error while binding INET socket!\n");
        exit(1);
    }
    status=listen(sock_fd, MAX_CLIENTS);
    if (status == -1){
        printf("Error in listen on INET!\n");
        exit(1);
    }

    printf("INET socket fd: %d\n", sock_fd);

    return sock_fd;

}

int check_clients(int unix_socket, int inet_socket){

    struct pollfd *fds = calloc(2 + no_clients, sizeof(struct pollfd));
    fds[0].fd = unix_socket;
    fds[0].events = POLLIN;
    fds[1].fd = inet_socket;
    fds[1].events = POLLIN;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < no_clients; i++){
        fds[i + 2].fd = clients[i]->fd;
        fds[i + 2].events = POLLIN;
    }

    pthread_mutex_unlock(&mutex);

    poll(fds, no_clients + 2, -1);
    int retfd;

    for (int i = 0; i < no_clients + 2; i++){
        if (fds[i].revents & POLLIN){
            retfd = fds[i].fd;
            break;
        }
    }
    if (retfd == unix_socket || retfd == inet_socket){
        retfd = accept(retfd, NULL, NULL);
    }
    free(fds);
    return retfd;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments!\n");
        exit(1);
    }
    srand(time(NULL));

    char *port = argv[1];
    char *socket_path = argv[2];

    int unix_socket = set_unix_socket(socket_path);
    int inet_socket = set_inet_socket(port);

    pthread_t pingthread;
    pthread_create(&pingthread, NULL, &ping, NULL);
    while (true) {
        int client_fd = check_clients(unix_socket, inet_socket);
        char buffer[MAX_MSG_LENGTH + 1];
        recv(client_fd, buffer, MAX_MSG_LENGTH, 0);
        printf("%s\n", buffer);
        char *command = strtok(buffer, ";");
        char *arg = strtok(NULL, ";");
        char *name = strtok(NULL, ";");

        pthread_mutex_lock(&mutex);
        if (strcmp(command, "add") == 0) {
            int index = add_client(name, client_fd);

            if (index == -1) {
                send(client_fd, "add;name_taken", MAX_MSG_LENGTH, 0);
                close(client_fd);
            }
            else if (index % 2 == 0) {
                send(client_fd, "add;no_opponent", MAX_MSG_LENGTH, 0);
            }
            else {
                int random_num = rand() % 100;
                int first, second;

                if (random_num % 2 == 0) {
                    first = index;
                    second = get_opponent(index);
                } else {
                    second = index;
                    first = get_opponent(index);
                }

                send(clients[first]->fd, "add;O",
                     MAX_MSG_LENGTH, 0);
                send(clients[second]->fd, "add;X",
                     MAX_MSG_LENGTH, 0);
            }
        }

        if (strcmp(command, "move") == 0) {
            int move = atoi(arg);
            int player = find_client(name);

            sprintf(buffer, "move;%d", move);
            send(clients[get_opponent(player)]->fd, buffer, MAX_MSG_LENGTH, 0);
        }
        if (strcmp(command, "end") == 0) {
            remove_client(name);
        }

        if (strcmp(command, "received") == 0) {
            int player = find_client(name);
            if (player != -1) {
                clients[player]->available = true;
            }

        }
        pthread_mutex_unlock(&mutex);
    }
}