#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 80

#define DEBUG 1

void shell_loop();
char *read_comm();
char *parse_comm(char *);

int main() {
    
    shell_loop(); 

    return 0;
}

void shell_loop() {
    int should_run = 1;
    while (should_run) {
        printf("gsh > ");
        fflush(stdout);

        char *comm = read_comm();
        parse_comm(comm);
    }
}

char *read_comm() {
    char *buff_ptr;
    size_t size = 0;
    getline(&buff_ptr, &size, stdin);
    if (strlen(buff_ptr) > MAX_LINE) {
        return NULL;
    } else {
        return buff_ptr;
    }
}

struct comm_element {
    char *el;
    struct comm_element *next;
};

char *parse_comm(char *raw_comm) {
    int start = 0;
    int char_count = 0;

    for (int offset = 0;; offset++) {
        if (raw_comm[offset] == NULL) {
            return NULL;
        }
        if (isblank((int)raw_comm[offset])) {
            printf("is space\n");
        }
    }
    
    return NULL;
}

