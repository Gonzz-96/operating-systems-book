#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct list_node {
    int n;
    struct list_node *next;
};

struct list_node *new_node();
struct list_node *collatz(int);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "collatz: missing arguments. Usage: ./collatz <n>, where n > 0\n");
        return 1;
    }
    
    char *nstr = argv[1];
    int n = atoi(nstr);

    if (n < 0) {
        fprintf(stderr, "collatz: argument n should conform to n > 0\n");
        return 1;
    }

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Failed to fork child process");
        return -1;
    } else if (pid == 0) { // child process
       struct list_node *root = collatz(n);
       struct list_node *curr = root;
       while (curr != NULL) {
            printf("%d ", curr->n);
            curr = curr->next;
       }
        printf("\n");
    } else { // parent process
        wait(NULL);
    }
    
    return  0;
}

struct list_node *collatz(int n) {
    struct list_node *root = (struct list_node *)malloc(sizeof(struct list_node));
    struct list_node *last = root;
    int cur = n;
    last->n = cur;

    while (cur != 1) {
        last->next = new_node();
        last = last->next;
        if (cur % 2 == 0) {
            cur = cur / 2;
        } else {
            cur = (3 * cur) + 1;
        }
        last->n = cur;
    }

    return root;
}


struct list_node *new_node() {
    return (struct list_node *)malloc(sizeof(struct list_node));    
}
