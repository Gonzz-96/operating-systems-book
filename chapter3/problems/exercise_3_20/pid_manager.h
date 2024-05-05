#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000

const static int pid_range = MAX_PID - MIN_PID;

struct gpid {
    int id;
    struct gpid *next;
};

struct pid_mgr {
    int cur_pid;
    char *pid_bitmap;
    struct gpid *last_pid;
    struct gpid *freed_pids;
};

static struct pid_mgr *pid_manager = NULL;

int allocate_map(void);
int allocate_pid(void);
void release_pid(int);

int allocate_map() {
   if (pid_manager) {
        return 1;
   } else {
        struct pid_mgr *manager = (struct pid_mgr *)malloc(sizeof(struct pid_mgr));
        manager->pid_bitmap = (char *)calloc(pid_range, sizeof(char));
        manager->freed_pids = NULL;
        manager->last_pid = NULL;
        manager->cur_pid = MIN_PID;
        pid_manager = manager;
        if (!pid_manager) {
            return -1;
        } else {
            return 1;
        }
   }
}

void free_map() {
    free(pid_manager->pid_bitmap);
    free(pid_manager->last_pid);
    struct gpid *id = pid_manager->freed_pids;
    while (id) {
        struct gpid *next = id->next;
        free(id);
        id = next;
    }
    free(pid_manager);
}

int allocate_pid() {
    if (!pid_manager && !pid_manager->pid_bitmap) {
        return -1;
    }
    if (!pid_manager->freed_pids) {
        int cur = pid_manager->cur_pid;
        pid_manager->pid_bitmap[cur - MIN_PID] = 1;
        pid_manager->cur_pid = cur + 1;
        return cur;
    } else {
        struct gpid *first = pid_manager->freed_pids;
        int id = first->id;
        pid_manager->freed_pids = first->next;
        pid_manager->pid_bitmap[id - MIN_PID] = 1;
        if (!pid_manager->freed_pids) {
            pid_manager->last_pid = NULL;
        }
        free(first);
        return id;
    }
}

void release_pid(int pid) {
    if (MAX_PID < pid || pid < MIN_PID) {
        return;
    }
    if (pid_manager->pid_bitmap[pid - MIN_PID] == 0) {
        return;
    }
    struct gpid *freed_pid = (struct gpid *)malloc(sizeof(struct gpid));
    freed_pid->id = pid;
    freed_pid->next = NULL;
    pid_manager->pid_bitmap[pid - MIN_PID] = 0;

    if (!pid_manager->freed_pids) {
        pid_manager->freed_pids = freed_pid;
        pid_manager->last_pid = freed_pid;
    } else {
        pid_manager->last_pid->next = freed_pid;
        pid_manager->last_pid = freed_pid;
    }
}

