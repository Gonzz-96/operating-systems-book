#include <stdio.h>
#include <stdlib.h>

#include "schedulers.h"
#include "task.h"

typedef struct _task_node {
   Task *current;
   struct _task_node *next;
} _TaskNode;

_TaskNode *_new_task_node(Task* task) {
    _TaskNode *new = (_TaskNode *)malloc(sizeof(_TaskNode));
    new->current   = task;
    new->next      = NULL;
    return new;
}

Task *_new_task(char *name, int burst) {
    Task *new_task  = (Task *)malloc(sizeof(Task));
    new_task->name  = name;
    new_task->burst = burst;
    return new_task;
}

_TaskNode *_head_node = NULL;
_TaskNode *_tail_node = NULL;

void add(char *name, int priority, int burst) {
    Task *new_task      = _new_task(name, burst);
    _TaskNode *new_node = _new_task_node(new_task);
    if (_head_node == NULL) {
        _tail_node = _head_node = new_node;
    } else {
        _tail_node->next = new_node;
        _tail_node = new_node;
    }
}

void schedule() {
    printf("The tasks will be executed in the next order:\n\n");
    printf("task name\tcpu burst\ttid\n");
    _TaskNode *current = _head_node;
    while (current != NULL) {
        Task *task = current->current;
        printf("%s\t\t%d\t\t%d\n", task->name, task->burst, task->tid);
        current = current->next;
    }
}
