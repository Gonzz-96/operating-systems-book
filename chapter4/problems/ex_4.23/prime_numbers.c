#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *primes(void *);
int is_prime(int);

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr, "primes: arg should be greated than zero\n");
        return 1;
    }

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, primes, &n);
    pthread_join(tid, NULL);

    return 0;
}

void *primes(void *args) {
    int n = *((int *)args);
    for (int i = 0; i < n; i++) {
        if (is_prime(i)) {
           printf("%d ", i); 
        }
    }
    printf("\n");
    pthread_exit(0);
}

int is_prime(int n) {
    if (n < 2) {
        return 0;
    }
    for (int i = 2; i * i < n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

