#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUF_SIZE 4096

static const char *shm_name = "OS";

struct list_node {
    int n;
    struct list_node *next;
};

struct list_node *new_node();
struct list_node *collatz(int);
void print_nodes(struct list_node *, int);

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

    int fd;
    char *ptr;
    fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, BUF_SIZE);
    ptr = (char *)mmap(0, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Failed to fork child process");
        return -1;
    } else if (pid == 0) { // child process
       struct list_node *root = collatz(n);
       print_nodes(root, fd);
    } else { // parent process
        wait(NULL);
        printf("collatz: reading from shmo\n");
        printf("%s\n", ptr);
        shm_unlink(shm_name);
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

void print_nodes(struct list_node *root, int fd) {
       struct list_node *curr = root;
       char buf[BUF_SIZE];
       buf[0] = '\0';
       while (curr != NULL) {
           char temp[10]; 
           sprintf(temp, "%d ", curr->n);
           strcat(buf, temp);
           curr = curr->next;
       }
       strcat(buf, "\n");
       write(fd, buf, strlen(buf));
}
