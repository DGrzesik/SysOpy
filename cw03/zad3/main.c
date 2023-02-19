#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>


void search_dir(char* dir_name, char* str, int d);

int main(int argc, char *argv[]) {
    if (argc!=4){
        printf("Wrong number of arguments!\n");
        return 1;
    }
    char *dir=(char*) calloc(256,sizeof(char));
    char *str;
    if (strcmp(argv[1],".")!=0 && strcmp(argv[1],"..")!=0){
        sprintf(dir, "%s%s","./", argv[1]);
    }
    else{
        dir=argv[1];
    }
    str=argv[2];
    int d = atoi(argv[3]);
    search_dir(dir,str,d);
    return 0;
}

void search_dir(char* dir_name, char* str, int d) {
    if(d==0) return;
    printf("sprawdzam path: %s\n",dir_name);
    struct dirent *dp;
    DIR *dir = opendir(dir_name);
    if (!dir) {
        printf("No such path!\n");
        exit(0);
    }

    while ((dp = readdir(dir))) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
            char* extension=calloc(sizeof(char), 4);
            char* path = calloc(sizeof(char), strlen(dir_name) + strlen(dp->d_name));
            sprintf(path, "%s/%s", dir_name, dp->d_name);
            extension = strrchr(dp->d_name,'.');
            struct stat *buf;
            pid_t childpid;
            buf = malloc(sizeof(struct stat));
            stat(path, buf);
            if (S_ISDIR(buf->st_mode) != 0 && (childpid=fork())==0){
                search_dir(path, str, d - 1);
                return;
            }
            if (extension!=NULL && strcmp(extension, ".txt") == 0) {
                FILE *file = fopen(path, "r");
                if (file == NULL) {
                    exit(1);
                }
                bool contains=false;
                char* line = calloc(256,sizeof(char));
                while(fgets(line, 256 * sizeof(char), file)){
                    if(strstr(line, str))
                    {
                        contains= true;
                        break;
                    }
                }
                fclose(file);
                free(line);
                if (contains){
                    printf("\npath: %s\n", path);
                    printf("PID:  %d \nPPID: %d\n", getpid(), getppid());
                }

            }
            free(path);
            while (wait(0) > 0);
        }
    closedir(dirq);
}