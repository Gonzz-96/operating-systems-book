#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

char **commargv(int, char **);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Missing arguments\n");
        return 1;
    }
    
    pid_t pid;
    pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "Fork failed, this program is panicking :(\n");
        return 1;
    } else if (pid == 0) {
        char *comm = argv[1];
        struct timeval init;
        
        // open_shmo();
        gettimeofday(&init, NULL);
        // the 'exec' command family will replace the image of the current process
        // with the information of the specified process. This means that any code
        // that is added after this line won't be executed as it won't be in
        // memory anymore. In order to inform the parent about the initial execution
        // time, an IPC mechanism (shared memory object or pipes) must be used
        execvp(comm, commargv(argc, argv));
    } else {
        wait(NULL);
    }

    return 0;
}

char **commargv(int argc, char *argv[]) {
    int nstrs = argc - 1;
    char **newargv = (char **)malloc(nstrs * sizeof(char *));
    for (int i = 0; i < nstrs; i++) {
        newargv[i] = (char *)malloc(strlen(argv[i+1]) * sizeof(char));
        newargv[i] = argv[i+1];
        printf("time: %s\n", newargv[i]);
    }
    return newargv;
}

