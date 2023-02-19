#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAX_ARGS 20
#define MAX_LINES 10
#define READ 0
#define WRITE 1

char **parse(char *line) {
    char **commands = (char **) calloc(MAX_ARGS, sizeof(char *));
    char *seq = strtok(line, "=");
    int ctr = 0;
    while ((seq = strtok(NULL, "|")) != NULL) {
        commands[ctr++] = seq;
    }
    return commands;
}

char **get_program_args(char *command, char *path) {
    char **args = (char **) calloc(256, sizeof(char *));
    char *arg = strtok(command, " ");
    strcpy(path, arg);
    int ctr = 0;
    args[ctr++] = arg;
    while ((arg = strtok(NULL, " ")) != NULL) {
        args[ctr++] = arg;
    }
    args[ctr] = NULL;
    return args;
}

int get_line_num(char *line, int i) {
    if (i == 0) return line[8] - '0';
    else return line[9] - '0';
}

int *get_lines_to_execute(char *line) {
    char **lines = (char **) calloc(MAX_ARGS, sizeof(char *));
    char *arg = strtok(line, "|");
    int ctr = 0;
    lines[ctr++] = arg;
    while ((arg = strtok(NULL, "|")) != NULL) {
        lines[ctr++] = arg;
        printf("arg%s\n", arg);
    }
    static int lines_num[MAX_ARGS];
    int i = 0;
    while (lines[i] != NULL) {
        lines_num[i] = get_line_num(lines[i], i);
        i++;
    }
    lines_num[i] = -1;
    return lines_num;
}

void parse_execute(FILE *file) {
    char **lines = (char **) calloc(MAX_LINES, sizeof(char *));
    char **commands;
    char **args;
    char *line = (char *) calloc(256, sizeof(char));
    char *current_line;
    int *lines_num;
    int line_ctr = 0;
    int lines_number;
    int i;
    while (fgets(line, 256 * sizeof(char), file)) {
        printf("\n--------------------------------------------");
        printf("\nLINE: %s", line);
        if (strstr(line, "=")) {
            char *line_copy = (char *) calloc(256, sizeof(char));
            strcpy(line_copy, line);
            lines[line_ctr++] = line_copy;
        } else {
            lines_num = get_lines_to_execute(line);
            printf("lines to execute: \n");
            i = 0;
            while (lines_num[i] != -1) {
                printf("line %d\n", lines_num[i]);
                i++;
            }
            int pipe_in[2];
            int pipe_out[2];
            if (pipe(pipe_out) != 0) {
                printf("Pipe error\n");
                exit(1);
            }
            lines_number = 0;
            while (lines_num[lines_number] != -1) lines_number++;
            for (i = 0; i < lines_number; i++) {
                current_line = lines[lines_num[i]];
                printf("\nexecuting line %d:  %s \n", i, current_line);
                commands = parse(lines[lines_num[i]]);
                int m = 0;
                while (commands[m] != NULL) {
                    printf("command%d:%s\n", m + 1, commands[m]);
                    m++;
                }
                for (int j = 0; j < m; j++) {
                    pid_t pid = fork();
                    if (pid == 0) {
                        if (j == 0 && i == 0) {
                            close(pipe_out[READ]);
                            dup2(pipe_out[WRITE], STDOUT_FILENO);
                        } else if (j == m - 1 && i == lines_number - 1) {
                            close(pipe_out[READ]);
                            close(pipe_out[WRITE]);
                            close(pipe_in[WRITE]);
                            dup2(pipe_in[READ], STDIN_FILENO);
                        } else {
                            close(pipe_in[WRITE]);
                            close(pipe_out[READ]);
                            dup2(pipe_in[READ], STDIN_FILENO);
                            dup2(pipe_out[WRITE], STDOUT_FILENO);
                        }
                        char path[256];
                        args = get_program_args(commands[j], path);
                        printf("----- EXEC ----- \npath: %s\n", path);
                        m = 0;
                        while (args[m] != NULL) {
                            printf("argument%d: %s\n", m + 1, args[m]);
                            m++;
                        }
                        if (execvp(path, args) == -1) {
                            printf("exec error\n");
                            exit(1);
                        }
                    } else {
                        close(pipe_in[WRITE]);
                        pipe_in[READ] = pipe_out[READ];
                        pipe_in[WRITE] = pipe_out[WRITE];
                        if (pipe(pipe_out) != 0) {
                            printf("Error while moving pipe\n");
                            exit(1);
                        }
                    }
                }
            }
        }
        int status = 0;
        pid_t wpid;
        while ((wpid = wait(&status)) != -1);
        printf("\nAll children terminated\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments!\n");
        exit(1);
    }
    char *path = argv[1];
    FILE *commands = fopen(path, "r");

    if (commands == NULL) {
        printf("Cannot open file\n");
        exit(1);
    }
    parse_execute(commands);
    fclose(commands);
}