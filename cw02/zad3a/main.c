#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

void listFilesRecursively(char *path, int *f, int *d, int *cd, int *bd, int *ff, int *sl, int *s);

int main(int argc, char *argv[]) {
    DIR *dir = opendir(argv[1]);
    if (!dir) {
        printf("No such path!\n");
        return 1;
    }
    int f = 0, d = 0, cd = 0, bd = 0, ff = 0, sl = 0, s = 0;
    listFilesRecursively(argv[1], &f, &d, &cd, &bd, &ff, &sl, &s);
    printf("Ilość zwykłych plików: %d\nIlość katalogów: %d\nIlość specjalnych plików znakowych: %d\n"
           "Ilość specjalnych plików blokowych: %d\nIlość potoków/kolejek FIFO: %d\nIlość linków symbolicznych: %d\n"
           "Ilość soketów: %d\n",f, d, cd, bd, ff, sl, s);
    return 0;
}

void listFilesRecursively(char *basePath, int *f, int *d, int *cd, int *bd, int *ff, int *sl, int *s) {

    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) {
        return;
    }
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            struct stat *buf;
            buf = malloc(sizeof(struct stat));
            stat(path, buf);
            printf("Dane o %s:\n", dp->d_name);
            printf("Ścieżka bezwzględna: %s\n", path);
            printf("Liczba dowiązań: %ld\n", buf->st_nlink);
            printf("Rodzaj pliku: ");
            if (S_ISREG(buf->st_mode) != 0) {
                printf("file\n");
                *f = *f + 1;
            }
            if (S_ISDIR(buf->st_mode) != 0) {
                printf("dir\n");
                *d = *d + 1;
            }
            if (S_ISCHR(buf->st_mode) != 0) {
                printf("char dev\n");
                *cd = *cd + 1;
            }
            if (S_ISBLK(buf->st_mode) != 0) {
                printf("block dev\n");
                *bd = *bd + 1;
            }
            if (S_ISFIFO(buf->st_mode) != 0) {
                printf("fifo\n");
                *ff = *ff + 1;
            }
            if (S_ISLNK(buf->st_mode) != 0) {
                printf("slink\n");
                *sl = *sl + 1;
            }
            if (S_ISSOCK(buf->st_mode) != 0) {
                printf("sock\n");
                *s = *s + 1;
            }
            printf("Rozmiar pliku: %ld\n", buf->st_size);
            printf("Ostatni dostęp do pliku: %s", ctime(&buf->st_atime));
            printf("Ostatnia modyfikacja pliku: %s\n", ctime(&buf->st_mtime));
            free(buf);
            listFilesRecursively(path, f, d, cd, bd, ff, sl, s);
        }
    }
    closedir(dir);
}