#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
    char *to=(char*)calloc(256, sizeof(char));
    if(argc==1){
        printf("Proszę podać nazwę pliku źródłowego: ");
        scanf("%s",from);
        printf("Proszę podać nazwę pliku docelowego: ");
        scanf("%s",to);
    }
    if(argc==2){
        from=argv[1];
        printf("Proszę podać nazwę pliku docelowego: ");
        scanf("%s",to);
    }
    else if(argc==3) {
        from=argv[1];
        to=argv[2];
    }
    else if(argc>3){
        printf("Too many arguments!\n");
        exit(1);
    }
    FILE *file_from = fopen(from, "rb");
    FILE *file_to = fopen(to, "w");
    fseek(file_from,0,SEEK_END);
    int size=ftell(file_from);
    fseek(file_from,0,SEEK_SET);
    char content[size];
    char processed_content[size];
    fread(content, sizeof(char), size, file_from);
    printf("\n");
    int j=0;
    int flag=0;
    int new_id=0;
    int last=0;
    char tmptab[size];
    for(int i=0;i<size;i++){
        if(!isspace(content[i])){
            flag=1;
        }
        if(content[i]==10){
            if(flag==1){
                tmptab[j]=content[i];
                j++;
                for(int a=0;a<j;a++){
                    processed_content[new_id]=tmptab[a];
                    new_id++;
                }
            }
            flag=0;
            j=0;
        }
        else{
            tmptab[j]=content[i];
            j++;

        }
        last=i;
    }
    if(flag){
        tmptab[j]=content[last];
        for(int a=0;a<j;a++){
            processed_content[new_id]=tmptab[a];
            new_id++;
        }
    }
    if(processed_content[new_id-1]==10 & processed_content[new_id-2]==13){
        new_id-=2;
    }
    fwrite(processed_content, sizeof(char), new_id, file_to);
    fclose(file_to);
    fclose(file_from);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);
    return 0;
}