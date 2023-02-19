#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <fcntl.h>

int main(void){
    mqd_t mq;
    struct mq_attr atr;
    int i;
    atr.mq_flags=0;
    atr.mq_maxmsg=100;
    atr.mq_msgsize= sizeof(int);
    atr.mq_curmsgs=0;
    mq = mq_open("./nazwa_kolejki",O_RDONLY);
    while(1){
        if (mq_receive(mq,(char*)&i, sizeof(int),NULL)<0){
            break;
        }
        printf("otrzymano %d\n",i);
    }
    mq_close(mq);
//    mq_unlink(QUEUE_NAME);
    return 0;
}