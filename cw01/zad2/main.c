#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>
#include "mylibrary.h"


long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}
int main(int argc, char *argv[]){
    struct tms start_tms;
    struct tms end_tms;
    struct tms start_tms_tot;
    struct tms end_tms_tot;
    clock_t clock_start;
    clock_t clock_end;
    clock_t total_clock_start;
    clock_t total_clock_end;
    struct MainArray *arr = NULL;
    total_clock_start = times(&start_tms_tot);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "create_table") == 0) {
            int size = atoi(argv[++i]);
            arr = create_table(size);
        }
        else if(strcmp(argv[i], "wc_files") == 0){
            char* file_name = (char*)calloc(strlen(argv[i]), sizeof(char));
            while(i < argc - 1 && strchr(argv[++i],'.')){
                int file_len = strchr(argv[i], '.') - argv[i];
                for(int a = 0; a < file_len + 4; a++){
                    file_name[a] = argv[i][a];
                }
                clock_start = times(&start_tms);
                wc_files(file_name);
                clock_end = times(&end_tms);
                print_res(clock_start, clock_end, start_tms, end_tms);
                clock_start = times(&start_tms);
                block_operation(arr);
                clock_end = times(&end_tms);
                print_res(clock_start, clock_end, start_tms, end_tms);

            }
            if(i != argc - 1) i--;
        }
        else if(strcmp(argv[i], "remove_block") == 0){
            clock_start = times(&start_tms);
            remove_block(arr,atoi(argv[++i]));
            clock_end = times(&end_tms);
            print_res(clock_start, clock_end, start_tms, end_tms);
        }

    }
    total_clock_end = times(&end_tms_tot);
    printf("TOTAL EXECUTION TIME\n");
    print_res(total_clock_start, total_clock_end, start_tms_tot, end_tms_tot);
    return 0;
}