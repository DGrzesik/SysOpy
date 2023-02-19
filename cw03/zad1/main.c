#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){

    char *m = (char*)calloc(256, sizeof(char));
    if(argc>2){
        printf("Too many arguments!\n");
        exit(1);
    }
    if (argc<2){
        printf("Podaj ilość procesów: ");
        scanf("%s",m);
    }
    if(argc==2){
        m=argv[1];
    }
    int n=atoi(m);
    for (int i=0;i<n;i++){
        pid_t child_pid;
        child_pid=fork();
        if (child_pid==0){
            printf("Proces macierzysty: %d\n",(int)getppid());
            return 0;
        }
    }
    return 0;
}