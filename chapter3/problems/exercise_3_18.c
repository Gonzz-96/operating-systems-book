#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t ch_pid;

    ch_pid = fork();

    if (ch_pid < 0) {
        fprintf(stderr, "Fork failed");
        return -1;
    } else if (ch_pid == 0) { // child code
        // a zombie child process is created when the child finishes
        // before the parent can wait for it. The 'sleep' statement
        // in the next else block gives time to the child to finish
        // and become a zombie to the OS
        return 1;
    } else { // parent code
        sleep(100);
        printf("Child complete\n");
    }

    return 0;
}

