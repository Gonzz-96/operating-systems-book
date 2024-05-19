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

}

void schedule() {

}
