#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(void){
    int status,gniazdo,dlugosc,nr=0,end=1,gniazdo2,c_end;
    struct sockaddr_in ser,cli;
    char buf[200];
    gniazdo = socket(AF_INET,SOCK_STREAM,0);
    if (gniazdo==-1){printf("blad socket\n");return 0;}
    memset(&ser,0,sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(4500);
    ser.sin_addr.s_addr = inet_addr("127.0.0.1");
    status = bind(gniazdo,(struct sockaddr *) &ser,sizeof ser);
    if(status==-1){printf("blad bind\n");return 0;}
    status = listen(gniazdo,10);
    if(status==-1){printf("blad listen\n");return 0;}
    dlugosc = sizeof cli;
    gniazdo2=accept(gniazdo,(struct sockaddr *)&cli,(socklen_t *)&dlugosc);
    c_end=1;
    if(gniazdo2==-1){printf("blad accept\n");return 0;}
    while(1){
        read(gniazdo2, buf, sizeof buf);
        if (buf[0] == 'Q') {
            sprintf(buf, "Serwer padł");
            write(gniazdo2, buf, strlen(buf));
            break;
        }
        else if (buf[0] == 'N') { sprintf(buf, "Jestes klientem nr %d", nr++); }
        else if (buf[0] == 'L') {
            sprintf(buf, "Rozłączono Cię z serwerem");
            write(gniazdo2, buf, strlen(buf));
            close(gniazdo2);
            dlugosc = sizeof cli;
            gniazdo2=accept(gniazdo,(struct sockaddr *)&cli,(socklen_t *)&dlugosc);
            if(gniazdo2==-1){printf("blad accept g2\n");return 0;}
        }
        else { sprintf(buf, "nie rozumiem pytania"); write(gniazdo2, buf, strlen(buf));}
        write(gniazdo2, buf, strlen(buf));
    }
    close(gniazdo);
}