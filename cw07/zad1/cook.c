#include "shared.h"


void place_in_oven(oven* o, int type){
    o->arr[o->place_id] = type;
    o->place_id++;
    o->place_id = o->place_id % OVEN_SIZE;
    o->pizzas++;
}

int take_out_pizza(oven* o){
    int type = o->arr[o->take_out_id];
    o->arr[o->take_out_id] = -1;
    o->take_out_id++;
    o->take_out_id = o->take_out_id % OVEN_SIZE;
    o->pizzas--;
    return type;
}


void place_on_table(table* t, int type){
    t->arr[t->place_id] = type;
    t->place_id++;
    t->place_id = t->place_id % OVEN_SIZE;
    t->pizzas++;
}

int main(){

    int sem_id = get_semaphore_set_id();
    int oven_sh_m_id = get_oven_shared_memory_id();
    int table_sh_m_id = get_table_shared_memory_id();

    oven* oven = shmat(oven_sh_m_id, NULL, 0);
    table* table = shmat(table_sh_m_id, NULL, 0);


    int type;

    srand(getpid());
    while(true){

        type = rand() % 10;
        printf("[COOK(pid: %d timestamp: %s)] -> Preparing pizza of type: %d\n", getpid(), get_current_time(), type);
        sleep(PREPARATION_TIME);
        lock_sem(sem_id, FULL_OVEN_SEM);
        lock_sem(sem_id, OVEN_SEM);
        place_in_oven(oven, type);
        printf("[COOK(pid: %d timestamp: %s)] ->  Added pizza of type %d to the oven. Number of pizzas baking: %d.\n", getpid(), get_current_time(), type, oven->pizzas);
        unlock_sem(sem_id, OVEN_SEM);


        sleep(BAKING_TIME);


        lock_sem(sem_id, OVEN_SEM);
        type = take_out_pizza(oven);
        printf("[COOK(pid: %d timestamp: %s)] -> Taking out pizza of type: %d. Number of pizzas baking: %d. Number of pizzas on the table: %d.\n", getpid(), get_current_time(), type, oven->pizzas, table->pizzas);
        unlock_sem(sem_id, OVEN_SEM);

        unlock_sem(sem_id, FULL_OVEN_SEM);

        lock_sem(sem_id, FULL_TABLE_SEM);

        lock_sem(sem_id, TABLE_SEM);
        place_on_table(table, type);
        printf("[COOK(pid: %d timestamp: %s)] -> Placed pizza of type %d on the table. Number of pizza baking: %d. Number of pizzas on the table: %d.\n", getpid(), get_current_time(), type, oven->pizzas, table->pizzas);
        unlock_sem(sem_id, TABLE_SEM);

        unlock_sem(sem_id, EMPTY_TABLE_SEM);

    }

}