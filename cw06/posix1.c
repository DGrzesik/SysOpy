#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
int main(void){
    struct mq_attr atr;
    int i;
    atr.mq_flags=0;
    atr.mq_maxmsg=100;
    atr.mq_msgsize= sizeof(int);
    atr.mq_curmsgs=0;
    mqd_t kolejka_kom =mq_open("./nazwa_kolejki",O_CREAT | O_WRONLY,0644,&atr);
    for (i=0;i<20;i++){
        mq_send(kolejka_kom,(const char*)&i,sizeof(int),0);
        //ostatni parametr to priorytet;nie moze przekraczac MQ_PRIO_MAX
    }
    mq_close(kolejka_kom);
    return 0;
}
//kompilacja -ltr