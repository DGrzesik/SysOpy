#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void){
    int w;
    key_t klucz=1;
    int id_kolejki_kom=msgget(klucz,0600);
    while(1){
        if (msgrcv(id_kolejki_kom,&w,sizeof(int),0,IPC_NOWAIT)<0){
            break;
        }
        printf("otrzymano: %d\n",w);
    }
    msgctl(id_kolejki_kom,IPC_RMID,NULL);//usunięcie kolejki
    return 0;
}