#include <stdio.h>
#include <stdlib.h>

#include "pid_manager.h"

struct test {
    int *arr;
};

int main() {

    //struct test *t = (struct test *)malloc(sizeof(struct test)); 
    //t->arr = (int *)calloc(10, sizeof(int));

    //t->arr[2] = 10;
    //t->arr[5] = 21;
    //t->arr[9] = 90;
    //printf("%d\n", t->arr[2]);
    //printf("%d\n", t->arr[5]);
    //printf("%d\n", t->arr[9]);

    allocate_map();
    printf("%p\n", pid_manager->pid_bitmap);
    
    for (int i = 0; i < 10; i++) {
        int pid = allocate_pid();
        printf("New allocated id: %d\n", pid);
    }

    release_pid(301);
    release_pid(305);
    release_pid(309);

    struct gpid *id = pid_manager->freed_pids;
    while (id) {
        printf("Freed id: %d\n", id->id);
        id = id->next;
    }

    for (int i = 0; i < 10; i++) {
        int pid = allocate_pid();
        printf("New allocated id: %d\n", pid);
    }

    free_map();
    return 0;
}

