#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct stats {
    int average;
    int min;
    int max;
};

struct thread_args {
    int *nums;
    int count;
    struct stats *result;
};

pthread_t *create_thread(struct thread_args *, void *(*)(void *));
int *parse_args(int, char **);
void *average(void *);
void *max    (void *);
void *min    (void *);

int main(int argc, char *argv[]) {
   struct thread_args targs;
   struct stats result;
   targs.nums  = parse_args(argc, argv);
   targs.count = argc - 1;
   targs.result = &result;

   pthread_t *tids[3];
   tids[0] = create_thread(&targs, average);
   tids[1] = create_thread(&targs, max);
   tids[2] = create_thread(&targs, min);

    for (int i = 0; i < 3; i++) {
        pthread_join(*tids[i], NULL);
    }

    printf("average: %d\n", result.average);
    printf("min: %d\n", result.min);
    printf("max: %d\n", result.max);

    for (int i = 0; i < 3; i++) {
        free(tids[i]);
    }

   return 0;
}

pthread_t *create_thread(struct thread_args *args, void *(*f)(void *)) {
    pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(tid, &attr, f, args);
    return tid;
}

void *average(void *args) {
    struct thread_args *targs = (struct thread_args *)args;
    struct stats *result = targs->result;
    int *nums = targs->nums;
    int sum = 0;
    for (int i = 0; i< targs->count; i++) {
        sum += nums[i];
    }
    result->average = sum / targs->count;
    pthread_exit(0);
}

void *max(void *args) {
    struct thread_args *targs = (struct thread_args *)args;
    struct stats *result = targs->result;
    int *nums = targs->nums;
    int max = INT_MIN;
    for (int i = 0; i < targs->count; i++) {
        if (nums[i] > max) {
            max = nums[i];
        }
    }
    result->max = max;
    pthread_exit(0);
}

void *min(void *args) {
    struct thread_args *targs = (struct thread_args *)args;
    struct stats *result = targs->result;
    int *nums = targs->nums;
    int min = INT_MAX;
    for (int i = 0; i < targs->count; i++) {
        if (nums[i] < min) {
            min = nums[i];
        }
    }
    result->min = min;
    pthread_exit(0);
}

int *parse_args(int count, char *argv[]) {
    int *intargs = (int *)malloc((count-1) * sizeof(int));
    for (int i = 0; i < count - 1; i++) {
        intargs[i] = atoi(argv[i+1]);
    }
    return intargs;
}

