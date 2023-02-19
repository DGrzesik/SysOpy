#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylibrary.h"

struct MainArray *create_table(int size){
    struct MainArray *arr = calloc(1, sizeof(struct MainArray));

    arr -> size = size;
    arr -> last_added = -1;
    arr -> blocks = calloc(size, sizeof(struct Block));
    return arr;
}
struct Block *create_block(){
    FILE *tmpf = fopen("tmpf.txt", "r");
    if(tmpf == NULL) {
        printf("Failed opening file\n");
        exit(1);
    }
    fseek(tmpf,0,SEEK_END);
    int size= ftell(tmpf);
    fseek(tmpf,0,SEEK_SET);
    struct Block *block = calloc(1, size);
    block->data =calloc(1, size);
    fgets(block->data, size, tmpf);
    fclose(tmpf);
    system("rm tmpf.txt");
    return block;
}

void wc_files(char* file_name){
    FILE *file = fopen(file_name, "r");

    if(file == NULL){
        perror("Failed opening file\n");
        exit(1);
    }
    char *fn = (char*)calloc(1, 256);
    strcat(fn, "wc < ");
    strcat(fn, file_name);
    strcat(fn, " > tmpf.txt");
    printf("Data loaded to temporary file\n");
    system(fn);
    free(fn);
}


int block_operation(struct MainArray* main_arr){
    struct Block* block= create_block();
    main_arr->blocks[main_arr->last_added + 1] = block;
    main_arr->last_added++;
    printf("Block contains %s\n",main_arr->blocks[main_arr->last_added]->data);
    return main_arr->last_added;
}


void remove_block(struct MainArray* arr, int id){
    if(arr->blocks[id] == NULL) return;
    free(arr->blocks[id]->data);
    free(arr->blocks[id]);
    arr->blocks[id] = NULL;
    printf("Block removed\n");
}