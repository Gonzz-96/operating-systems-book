#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *fib_gen(void *);
void fib(int);

static int *fib_seq;

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    fib_seq = (int *)malloc(n * sizeof(int));

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, fib_gen, &n);

    pthread_join(tid, NULL);

    for (int i = 0; i < n; i++) {
        printf("%d ", fib_seq[i]);
    }
    printf("\n");
    
    return 0;
}

void *fib_gen(void *args) {
    int n = *((int *)args);
    fib(n);
    pthread_exit(0);    
}

void fib(int n) {

    for (int i = 0; i < n; i++) {
        if (i >= 2) {
            fib_seq[i] = fib_seq[i-1] + fib_seq[i-2];
        } else if (i == 1) {
            fib_seq[i] = 1;
        } else {
            fib_seq[i] = 0;
        }
    }
}

