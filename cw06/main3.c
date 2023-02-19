#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct pakiet{
    long typ; //pierwsze pole musi byc long i stanowic typ
    int zawartosc; //dowolna zwartosc, rozmiar struktury <= MSGMAX(4096)
    //char zaw_inna[100]
};
int main(void){
    int i;
    struct pakiet o1;
    key_t klucz = 1;//plik, jednorazowy identyfiktor projektu
    int id_kolejki_kom=msgget(klucz,IPC_CREAT | 0600);
    for (i=0;i<20;i++){
        o1.typ=(i%2)+1;//typ>0
        o1.zawartosc=i;
        msgsnd(id_kolejki_kom,&o1,sizeof(struct pakiet),0);
    }
    return 0;
}
//ipsc -q