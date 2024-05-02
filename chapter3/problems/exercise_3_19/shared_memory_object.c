#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "command.h"

static const short SHMO_SIZE = 4096;
static const char *SHMO_NAME = "OS";

struct ShMObj *open_shmo(const char *, int); 
void write_to_shmo(struct ShMObj *, struct timeval *);

struct ShMObj {
    int fd;
    char *ptr;
};

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Missing arguments\n");
        return 1;
    }
    
    pid_t pid;
    pid = fork();

    struct ShMObj* shmo = NULL;
    
    if (pid < 0) {
        fprintf(stderr, "Fork failed, this program is panicking :(\n");
        return 1;
    } else if (pid == 0) {
        char *comm = argv[1];
        struct timeval init;
        shmo = open_shmo(SHMO_NAME, SHMO_SIZE);
        gettimeofday(&init, NULL);
        write_to_shmo(shmo, &init);       
        // the 'exec' command family will replace the image of the current process
        // with the information of the specified process. This means that any code
        // that is added after this line won't be executed as it won't be in
        // memory anymore. In order to inform the parent about the initial execution
        // time, an IPC mechanism (shared memory object or pipes) must be used
        // Also, code beyond this point is exectued only if the execvp encounters
        // an error. In such case, it'll return -1
        int err = execvp(comm, commargv(argc, argv));
        if (err == -1) {
            fprintf(stderr, "time: failed to execute command \"%s\"\n", comm);
            return -1;
        }
    } else {
        wait(NULL);
        
        shmo = open_shmo(SHMO_NAME, SHMO_SIZE);
        char *initstr = shmo->ptr;
        printf("time: parent -> %s\n", initstr); 

        struct timeval end;
        gettimeofday(&end, NULL);

        int init = atoi(initstr);
        printf("Elapsed time: %ds\n", end.tv_sec - init);

        shm_unlink(SHMO_NAME);
    }

    return 0;
}

struct ShMObj *open_shmo(const char *name, int size) {
    struct ShMObj *shmo = (struct ShMObj *)malloc(sizeof(struct ShMObj));

    // for a shared memory object to be opened correctly, it is needed that
    // the shm_open is called with the exact same parameters in all the involved
    // processes
    shmo->fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shmo->fd, size);
    shmo->ptr = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shmo->fd, 0);

    return shmo;
}

void write_to_shmo(struct ShMObj *shmo, struct timeval *init) {
    char *bf = (char *)malloc(200 * sizeof(char));
    sprintf(bf, "%d", init->tv_sec);
    sprintf(shmo->ptr, "%s", bf); 
    shmo->ptr += strlen(bf);
    printf("time: child -> %s %d\n", bf, strlen(bf));
    free(bf);
}

