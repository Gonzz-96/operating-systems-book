#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREAD 5

void *runner(void *);

int main(int argc, char *argv[]) {

    int i;
    int policy;
    pthread_t tid[NUM_THREAD];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    if (pthread_attr_getschedpolicy(&attr, &policy) != 0) {
        fprintf(stderr, "Unable to get scheduling policy\n");
    } else {
        switch (policy) {
        case SCHED_OTHER:
            printf("SCHED_OTHER\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR\n");
            break;
        case SCHED_FIFO:
            printf("SCHED_FIFO\n");
            break;
        default:
            printf("No supported policy\n");
        }
    }

    if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0) {
        fprintf(stderr, "Unable to set policy.\n");
    }

    for (int i = 0; i < NUM_THREAD; i++) {
        int num_thread = i + 1;
        pthread_create(&tid[i], &attr, runner, &num_thread);
    }

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(tid[i], NULL);
    }
}

void *runner(void *args) {
    int t_num = *((int *) args);
    printf("Hello from thread #%d\n", t_num);
    sleep(2);
    pthread_exit(0);
}
