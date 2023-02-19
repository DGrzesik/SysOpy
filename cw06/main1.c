#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(void){
    int i;
    key_t klucz = 1;//plik, jednorazowy identyfiktor projektu
    int id_kolejki_kom=msgget(klucz,IPC_CREAT | 0600);
    for (i=0;i<20;i++){
        msgsnd(id_kolejki_kom,&i,sizeof(int),0);
    }
    return 0;
}
//ipsc -q