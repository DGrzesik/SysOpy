#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylibrary.h"

struct MainArray *create_main_array(int size){
    struct MainArray *arr = calloc(1, sizeof(struct MainArray));

    arr -> size = size;
    arr -> last_added = -1;
    arr -> blocks = calloc(size, sizeof(struct Block));
    printf("Array created succesfully\n");

    return arr;
}


void wc_files(char* file_name){
    FILE *file = fopen(file_name, "r");

    if(file == NULL){
        perror("Cannot open file");
        exit(1);
    }

    //FILE *tmpf = fopen("tmpf.txt", "w");

//    system("wc < "file_name "| tee "tmpf);

    char *fn = (char*)calloc(1, 16 + sizeof(file_name));
//    strcat(fn, "wc < ",file_name ," > tmpf.txt");
    strcat(fn, "wc < ");
    strcat(fn, file_name);
    strcat(fn, " > tmpf.txt");
    printf("%s\n", fn);
    system(fn);
    free(fn);
}


int block_operation(struct MainArray* main_arr){

    FILE *tmpf = fopen("tmpf.txt", "r");
    if(tmpf == NULL) {
        printf("chuj nie działa");
        exit(1);
    }
    struct Block *block = calloc(1, sizeof(tmpf));
    char* line = (char*) calloc(1,sizeof(tmpf));
    fgets(line, sizeof(tmpf), tmpf);
    block->data = line;
    main_arr->blocks[main_arr->last_added + 1] = block;
    main_arr->last_added++;
    printf("Added block at index %d\n", main_arr->last_added);
    printf("block contains %s\n",main_arr->blocks[main_arr->last_added]->data);
    fclose(tmpf);
    system("rm tmpf.txt");
    free(line);
    return main_arr->last_added;
}


void delete_block(struct MainArray* arr, int id){
    if(arr->blocks[id] == NULL) return;
    free(arr->blocks[id]);
    arr->blocks[id] = NULL;
    printf("Block removed");
}