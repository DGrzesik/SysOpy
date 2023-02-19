#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define NO_REINDEER 9
#define NO_REINDEER_ON_HOLIDAY rand() % 6 + 5
#define REQ_NO_ELVES_WAITING_FOR_SANTA 3
#define DELIVERING_TOYS_TIME rand() % 3 + 2

#define NO_ELVES 10
#define ELF_WORKING_TIME rand() % 4 + 2

#define SANTA_PROBLEM_SOLVING_TIME rand() % 2 + 1

bool reindeer_waiting = false;
int no_reindeer_back = 0;
int no_deliveries = 0;

int no_elves_waiting_for_santa = 0;
int elves_queue[REQ_NO_ELVES_WAITING_FOR_SANTA];

pthread_mutex_t mutex_r = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_r_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_r_wait = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_e = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_e_wait = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_e_wait = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_s = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_s = PTHREAD_COND_INITIALIZER;



void *santa(void *arg) {
    while (true) {
        pthread_mutex_lock(&mutex_s);

        //mikołaj czeka na przebudzenie przez elfy lub renifery
        while (no_reindeer_back < NO_REINDEER && no_elves_waiting_for_santa < REQ_NO_ELVES_WAITING_FOR_SANTA) {
            pthread_cond_wait(&cond_s, &mutex_s);
        }
        pthread_mutex_unlock(&mutex_s);

        printf("Mikołaj: budzę się\n");

        pthread_mutex_lock(&mutex_r);
        //mikołaj dostarcza zabawki
        if (no_reindeer_back == NO_REINDEER) {
            no_deliveries++;
            printf("Mikołaj: dostarczam zabawki [po raz %d]\n", no_deliveries);
            sleep(DELIVERING_TOYS_TIME);
            no_reindeer_back = 0;

            // renifery przestają czekać na mikołaja
            pthread_mutex_lock(&mutex_r_wait);
            reindeer_waiting = false;
            pthread_cond_broadcast(&cond_r_wait);
            pthread_mutex_unlock(&mutex_r_wait);
        }
        pthread_mutex_unlock(&mutex_r);

        //w momencie dostarczenia zabawek po raz 3, program zostaje zakończony
        if (no_deliveries == 3) break;

        //mikołaj rozwiązuje problemy elfów
        pthread_mutex_lock(&mutex_e);
        if (no_elves_waiting_for_santa == REQ_NO_ELVES_WAITING_FOR_SANTA) {
            printf(" Mikołaj: rozwiązuje problemy elfów: %d %d %d\n", elves_queue[0], elves_queue[1], elves_queue[2]);
            sleep(SANTA_PROBLEM_SOLVING_TIME);

            // kolejka elfów do mikołaja zwalnia się
            pthread_mutex_lock(&mutex_e_wait);
            no_elves_waiting_for_santa = 0;
            pthread_cond_broadcast(&cond_e_wait);
            pthread_mutex_unlock(&mutex_e_wait);

        }
        pthread_mutex_unlock(&mutex_e);

        printf("Mikołaj: zasypiam\n");
    }

    exit(0);
}

void *reindeer(void *arg) {
    int ID = *((int *) arg);
    while (true) {
        pthread_mutex_lock(&mutex_r_wait);
        while (reindeer_waiting) {
            pthread_cond_wait(&cond_r_wait, &mutex_r_wait);
        }
        pthread_mutex_unlock(&mutex_r_wait);
        sleep(NO_REINDEER_ON_HOLIDAY);
        pthread_mutex_lock(&mutex_r);
        no_reindeer_back++;
        printf("Renifer: czeka %d reniferów na Mikołaja, %d\n", no_reindeer_back, ID);
        reindeer_waiting = true;
        if (no_reindeer_back == NO_REINDEER) {
            printf("Renifer: wybudzam Mikołaja, %d\n", ID);
            pthread_mutex_lock(&mutex_s);
            pthread_cond_broadcast(&cond_s);
            pthread_mutex_unlock(&mutex_s);
        }
        pthread_mutex_unlock(&mutex_r);
    }
}


void *elf(void *arg) {
    int ID = *((int *) arg);
    while (true) {
        sleep(ELF_WORKING_TIME);
        pthread_mutex_lock(&mutex_e_wait);
        while (no_elves_waiting_for_santa == REQ_NO_ELVES_WAITING_FOR_SANTA) {
            printf("Elf: czeka na powrót elfów, %d\n", ID);
            pthread_cond_wait(&cond_e_wait, &mutex_e_wait);
        }
        pthread_mutex_unlock(&mutex_e_wait);
        pthread_mutex_lock(&mutex_e);
        if (no_elves_waiting_for_santa < REQ_NO_ELVES_WAITING_FOR_SANTA) {
            elves_queue[no_elves_waiting_for_santa] = ID;
            no_elves_waiting_for_santa++;
            printf("Elf: czeka %d elfów na Mikołaja, %d\n", no_elves_waiting_for_santa, ID);
            if (no_elves_waiting_for_santa == REQ_NO_ELVES_WAITING_FOR_SANTA) {
                printf("Elf: wybudzam Mikołaja, %d\n", ID);
                pthread_mutex_lock(&mutex_s);
                pthread_cond_broadcast(&cond_s);
                pthread_mutex_unlock(&mutex_s);
            }
        }
        pthread_mutex_unlock(&mutex_e);
    }
}


int main() {
    srand(time(NULL));

    //kolejka elfów do Mikołaja
    for (int i = 0; i < REQ_NO_ELVES_WAITING_FOR_SANTA; i++) {
        elves_queue[i] = -1;
    }

    // wątek mikołaja
    pthread_t santa_t;
    pthread_create(&santa_t, NULL, &santa, NULL);

    // tablica wątków reniferów oraz tablica ich ID
    int *reindeer_ID = calloc(NO_REINDEER, sizeof(int));
    pthread_t *reindeer_t = calloc(NO_REINDEER, sizeof(pthread_t));
    for (int i = 0; i < NO_REINDEER; i++) {
        reindeer_ID[i] = i;
        pthread_create(&reindeer_t[i], NULL, &reindeer, &reindeer_ID[i]);
    }

    // tablica wątków elfów oraz tablica ich ID
    int *elves_ID = calloc(NO_ELVES, sizeof(int));
    pthread_t *elves_t = calloc(NO_ELVES, sizeof(pthread_t));
    for (int i = 0; i < NO_ELVES; i++) {
        elves_ID[i] = i;
        pthread_create(&elves_t[i], NULL, &elf, &elves_ID[i]);
    }

    //oczekiwanie na zakończenie wszystkich wątków

    pthread_join(santa_t, NULL);

    for (int i = 0; i < NO_REINDEER; i++) {
        pthread_join(reindeer_t[i], NULL);
    }

    for (int i = 0; i < NO_ELVES; i++) {
        pthread_join(elves_t[i], NULL);
    }


    return 0;
}