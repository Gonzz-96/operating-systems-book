#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5

void *runner(void *);

int main(int argc, char *argv[]) {
    int scope;

    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    if (pthread_attr_getscope(&attr, &scope) != 0) {
        fprintf(stderr, "Unable to get scheduling scope\n");
    } else  {
        switch (scope) {
        case PTHREAD_SCOPE_PROCESS:
            printf("PTHREAD_SCOPE_PROCESS\n");
            break;
        case PTHREAD_SCOPE_SYSTEM: // only scheduling supported by Linux
            printf("PTHREAD_SCOPE_SYSTEM\n");
            break;
        deafult:
            fprintf(stderr, "Illegal scope value\n");
        }
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int t_num = i + 1;
        pthread_create(&tid[i], &attr, runner, &t_num);
    } 

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Threads finished running\n");

    return 0;
}

void *runner(void *args) {
    int t_num = *((int *) args);
    printf("Hello from thread #%d\n", t_num);
    sleep(5);
    pthread_exit(0);
}

