#include "shared.h"


int take_pizza_from_table(table* t){
    int type = t->arr[t->take_out_id];
    t->arr[t->take_out_id] = -1;
    t->take_out_id++;
    t->take_out_id = t->take_out_id % TABLE_SIZE;
    t->pizzas--;
    return type;
}

int main(){

    int sem_id = get_semaphore_set_id();
    int shm_table_id = get_table_shared_memory_id();

    table* table = shmat(shm_table_id, NULL, 0);

    int type;

    while(true) {
        lock_sem(sem_id, EMPTY_TABLE_SEM);

        lock_sem(sem_id, TABLE_SEM);
        type = take_pizza_from_table(table);
        printf("[DELIVERY_MAN(pid: %d timestamp: %s)] -> Taking pizza of type: %d. Number of pizzas on the table: %d.\n", getpid(), get_current_time(), type, table->pizzas);
        unlock_sem(sem_id, TABLE_SEM);

        unlock_sem(sem_id, FULL_TABLE_SEM);

        sleep(DELIVERY_TIME);

        printf("[DELIVERY_MAN(pid: %d timestamp: %s)] -> Pizza of type %d delivered.\n", getpid(), get_current_time(), type);
        sleep(RETURN_TIME);

    }


}