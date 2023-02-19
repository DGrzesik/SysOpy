#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/times.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}
int main(int argc, char* argv[]){
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    clock_start = times(&start_tms);
    char *from = (char*)calloc(256, sizeof(char));
    char *c= (char*) calloc(1, sizeof(char));
    if (argc<3){
        printf("Too few arguments!\n");
    }
    else if(argc==3){
        from=argv[2];
        c=argv[1];
    }
    else if(argc>3){
        printf("Too many arguments!\n");
        exit(1);
    }
    int file_from = open(from, O_RDONLY);
    int size=lseek(file_from,0,SEEK_END);
    lseek(file_from,0,SEEK_SET);
    char content[size];
    read(file_from,content,size);
    int flag=0;
    int lines=0;
    int apprncs=0;
    for(int i=0;i<size;i++){
        if(content[i]==c[0]){
            apprncs++;
            flag=1;
        }
        if(content[i]==10){
            if(flag==1){
                lines++;
            }
            flag=0;
        }
    }
    if(flag==1){
        lines++;
    }
    printf("Wynik: appearances: %d, lines: %d\n",apprncs,lines);
    close(file_from);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);
    return 0;
}