#include <stdio.h>

struct Block{
    char* data;
};
struct MainArray{
    int size;
    int last_added;
    struct Block **blocks;
};

struct MainArray *create_table(int size);

void wc_files(char* file_name);

int block_operation( struct MainArray* main_arr);

void remove_block(struct MainArray* arr, int id);
