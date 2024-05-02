#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "command.h"

#define BUFFER_SIZE 200
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {
    
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe creation failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        // error
        return -1;
    } else if (pid == 0) { // child process
        close(fd[READ_END]);
        struct timeval init;
        gettimeofday(&init, NULL);
        char *initstr;
        sprintf(initstr, "%d", init.tv_sec);
        write(fd[WRITE_END], initstr, strlen(initstr)+1);
        close(fd[WRITE_END]);
        char *comm = argv[1];

        printf("%s\n", comm);
         
        int err = execvp(comm, commargv(argc, argv));
        if (err == -1) {
            fprintf(stderr, "time: failed to execute command \"%s\"\n", comm);
            return -1;
        }
    } else { // parent process
        char buffer[BUFFER_SIZE];
        close(fd[WRITE_END]);
        read(fd[READ_END], buffer, BUFFER_SIZE);
        printf("time: %d\n", buffer);
        close(fd[READ_END]);
    }

    return 0;
}

