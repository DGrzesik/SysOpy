#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <string.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}
int main(int argc,char *argv[]){
    char *w = (char*)calloc(256, sizeof(char));
    char *q = (char*)calloc(256, sizeof(char));
    if(argc>3){
        printf("Too many arguments!\n");
        return 0;
    }
    if (argc==1){
        printf("Podaj szerokość prostokąta: ");
        scanf("%s",w);
        printf("Podaj ilość procesów: ");
        scanf("%s",q);
    }
    if (argc==2){
        w=argv[1];
        printf("Podaj ilość procesów: ");
        scanf("%s",q);
    }
    if(argc==3){
        w=argv[1];
        q=argv[2];
    }
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    clock_start = times(&start_tms);
    int n=atoi(q);
    float width=atof(w);
    int part=(int)(1/width);
    int perchild=(int)ceilf((float)part/(float)n);
    if (perchild<1){
        perchild=1;
    }
    int i=1;
    int itr=1;
    while (itr<=part){
        pid_t child_pid;
        child_pid=fork();
        if(child_pid==0){
            char *currfile= (char*)calloc(256, sizeof(char));
            char *count=(char*)calloc(256, sizeof(char));
            sprintf(count, "%d", i);
            strcat(currfile,"w");
            strcat(currfile,count);
            strcat(currfile,".txt");
            FILE *file = fopen(currfile,"w");
            int done=0;
            while(done<perchild){
                float x_i=(float)itr*width;
                int n_dig = 10;
                char result[n_dig];
                float x_2_i = powf(x_i, 2);
                float f_i = (float)4 /(x_2_i + 1);
                gcvt(f_i,n_dig,result);
                fprintf(file,"%s", result);
                fprintf(file, "\n");
                itr+=1;
                done++;
            }
            free(currfile);
            free(count);
            fclose(file);
            return 0;
        }
        i=i+1;
        itr=itr+perchild;
    }
    while (wait(0) > 0);
    n=i-1;
    float reimann=0;
    char content[256];
    i=1;
    while(i<=n){
        char *currfile= (char*)calloc(256, sizeof(char));
        char *count=(char*)calloc(256, sizeof(char));
        sprintf(count, "%d", i);
        strcat(currfile,"w");
        strcat(currfile,count);
        strcat(currfile,".txt");
        FILE *file_from = fopen(currfile, "rb");
        while(fgets(content, 256, file_from)) {
            reimann += atof(content);
        }
        free(currfile);
        free(count);
        fclose(file_from);
        i+=1;
    }
    printf("%f\n",reimann*width);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);
    return 0;
}