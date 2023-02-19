#include "shared.h"

int server_socket;
int is_client_O;
char buffer[MAX_MSG_LENGTH + 1];
char *name, *command, *arg;
game_board board;
state current_state = START;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


int move(game_board *game_board, int position) {
    if (position < 0 || position > 9 || game_board->slots[position] != EMPTY) {
        return 0;
    }
    game_board->slots[position] = game_board->move ? O : X;
    game_board->move = !game_board->move;
    return 1;
}

slot check_winner(game_board *game_board) {
    slot column = EMPTY;

    for (int x = 0; x < 3; x++) {
        slot first = game_board->slots[x];
        slot second = game_board->slots[x + 3];
        slot third = game_board->slots[x + 6];

        if (first == second && first == third && first != EMPTY) {
            column = first;
        }
    }

    if (column != EMPTY) {
        return column;
    }

    slot row = EMPTY;
    for (int y = 0; y < 3; y++) {
        slot first = game_board->slots[3 * y];
        slot second = game_board->slots[3 * y + 1];
        slot third = game_board->slots[3 * y + 2];
        if (first == second && first == third && first != EMPTY)
            row = first;
    }

    if (row != EMPTY) {
        return row;
    }

    slot dsc_diagonal = EMPTY;

    slot first = game_board->slots[0];
    slot second = game_board->slots[4];
    slot third = game_board->slots[8];

    if (first == second && first == third && first != EMPTY) {
        dsc_diagonal = first;
    }
    if (dsc_diagonal != EMPTY) {
        return dsc_diagonal;
    }

    slot asc_diagonal = EMPTY;

    first = game_board->slots[2];
    second = game_board->slots[4];
    third = game_board->slots[6];

    if (first == second && first == third && first != EMPTY) {
        asc_diagonal = first;
    }

    return asc_diagonal;
}

void end() {
    char buffer[MAX_MSG_LENGTH + 1];
    sprintf(buffer, "end; ;%s", name);
    send(server_socket, buffer, MAX_MSG_LENGTH, 0);
    exit(0);
}

void draw_board() {
    char symbol;
    printf("\n_______________\n");
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board.slots[y * 3 + x] == EMPTY) {
                symbol = y * 3 + x + 1 + '0';
            }
            else if (board.slots[y * 3 + x] == O) {
                symbol = 'O';
            }
            else {
                symbol = 'X';
            }
            printf("  %c |", symbol);
        }
        printf("\n_______________\n");
    }
}

void check_game_status() {
    bool win = false;

    slot winner = check_winner(&board);

    if (winner != EMPTY) {
        if ((is_client_O && winner == O) || (!is_client_O && winner == X)) {
            printf("[%c] Victory!\n",is_client_O ? 'O' : 'X');
        }
        else {
            draw_board();
            printf("[%c] Defeat!\n",is_client_O ? 'O' : 'X');
        }

        win = true;
    }

    bool draw = true;
    for (int i = 0; i < 9; i++) {
        if (board.slots[i] == EMPTY) {
            draw = false;
            break;
        }
    }

    if (draw && !win) {
        printf("Draw!\n");
    }

    if (win || draw) {
        current_state = QUIT;
    }
}

void parse_command(char *msg) {
    command = strtok(msg, ";");
    arg = strtok(NULL, ";");
}

game_board create_board() {
    game_board game_board = {1, {EMPTY}};
    return game_board;
}


void play_game() {
    while (true) {
        if (current_state == START) {
            if (strcmp(arg, "name_taken") == 0) {
                exit(1);
            }
            else if (strcmp(arg, "no_opponent") == 0) {
                current_state = WAIT_FOR_OPPONENT;
            }
            else {
                board = create_board();
                is_client_O = arg[0] == 'O';
                current_state = is_client_O ? MOVE : WAIT_FOR_MOVE;
            }
        }
        else if (current_state == WAIT_FOR_OPPONENT) {

            pthread_mutex_lock(&mutex);
            while (current_state != START && current_state != QUIT) {
                printf("Waiting for opponent!\n");
                pthread_cond_wait(&cond, &mutex);
            }

            pthread_mutex_unlock(&mutex);

            board = create_board();
            is_client_O = arg[0] == 'O';
            current_state = is_client_O ? MOVE : WAIT_FOR_MOVE;
        }
        else if (current_state == WAIT_FOR_MOVE) {
            printf("Opponents move!\n");

            pthread_mutex_lock(&mutex);
            while (current_state != OPPONENT_MOVE && current_state != QUIT) {
                pthread_cond_wait(&cond, &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
        else if (current_state == OPPONENT_MOVE) {
            int pos = atoi(arg);
            move(&board, pos);
            check_game_status();
            if (current_state != QUIT) {
                current_state = MOVE;
            }
        }
        else if (current_state == MOVE) {
            draw_board();
            int pos;
            do {
                printf("[%c] Your move: ", is_client_O ? 'O' : 'X');
                scanf("%d", &pos);
                pos--;
            } while (!move(&board, pos));

            draw_board();

            sprintf(buffer, "move;%d;%s", pos, name);
            send(server_socket, buffer, MAX_MSG_LENGTH, 0);

            check_game_status();
            if (current_state != QUIT) {
                current_state = WAIT_FOR_MOVE;
            }
        }
        else if (current_state == QUIT) {
            end();
        }
    }
}

void connect_local(char *path) {
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    int status;
    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, path);
    status = connect(server_socket, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    if (status == -1) {
        printf("Error while connecting to LOCAL socket (%s)!\n", strerror(errno));
        exit(1);
    }
}

void connect_inet(char *port) {
    int sock_fd,status;
    struct sockaddr_in addr;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    status=connect(server_socket, (struct sockaddr*) &addr, sizeof addr);
    if (status < 0) {
        printf("Error while connecting to INET socket (%s)!\n", strerror(errno));
        exit(1);
    }
}

void listen_server() {
    int game_thread_active = 0;
    while (1) {
        recv(server_socket, buffer, MAX_MSG_LENGTH, 0);
        parse_command(buffer);
        pthread_mutex_lock(&mutex);
        if (strcmp(arg,"name_taken")==0){
            printf("Name taken!\n");
            exit(1);
        }
        else if (strcmp(command, "add") == 0) {
            current_state = START;
            if (!game_thread_active) {
                pthread_t t;
                pthread_create(&t, NULL, (void *(*)(void *)) play_game, NULL);
                game_thread_active = 1;
            }
        }
        else if (strcmp(command, "move") == 0) {
            current_state = OPPONENT_MOVE;
        }

        else if (strcmp(command, "end") == 0) {
            current_state = QUIT;
            exit(0);
        }
        else if (strcmp(command, "ping") == 0) {
            sprintf(buffer, "received; ;%s", name);
            send(server_socket, buffer, MAX_MSG_LENGTH, 0);
        }
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Wrong number of arguments!\n");
        exit(1);
    }
    name = argv[1];
    signal(SIGINT, end);
    if (strcmp(argv[2], "unix") == 0) {
        char *path = argv[3];
        connect_local(path);
    }
    else if (strcmp(argv[2], "inet") == 0) {
        char *port = argv[3];
        connect_inet(port);
    }
    else {
        printf("Choose 'inet' or 'unix' method!\n");
        exit(1);
    }

    char message[MAX_MSG_LENGTH];
    sprintf(message, "add; ;%s", name);


    send(server_socket, message, MAX_MSG_LENGTH, 0);

    listen_server();

    return 0;
}