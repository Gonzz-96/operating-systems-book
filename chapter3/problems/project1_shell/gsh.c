#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define ARGS_DELIMITER " "

#define DEBUG 1

void shell_loop();
char *read_comm();
char **tokenize(char *);
struct comm_token *new_token();

int main() {
    
    shell_loop(); 

    return 0;
}

void shell_loop() {
    int should_run = 1;
    char **tokenized_command;
    char **previous_command;
    while (should_run) {
        printf("gsh > ");
        fflush(stdout);

        char *comm = read_comm();
        tokenized_command = tokenize(comm);
        if (tokenized_command != NULL && !strcmp(tokenized_command[0], "exit")) {
            exit(0);
        } else if (!strcmp(tokenized_command[0], "!!")) {
            tokenized_command = previous_command;
        }

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed");
            exit(1);
        } else if (pid == 0) { // child process
            execvp(tokenized_command[0], tokenized_command);
            exit(errno);
        } else {
            int exit_status;
            wait(&exit_status);
            if (exit_status != 0) {
                fprintf(stderr, "gsh: command not found: %s\n", tokenized_command[0]);
            }
            previous_command = tokenized_command;
        }
    }
}

char *read_comm() {
    char *buff_ptr = NULL;
    size_t size = 0;
    getline(&buff_ptr, &size, stdin);
    if (strlen(buff_ptr) > MAX_LINE) {
        return NULL;
    } else {
        // getline includes the newline character at the end
        // it needs to be supressed
        buff_ptr[strlen(buff_ptr)-1] = '\0';
        return buff_ptr;
    }
}

struct comm_token {
    char *str_token;
    struct comm_token *next;
};



char **tokenize(char *raw_comm) {
    char *comm = (char *)malloc(strlen(raw_comm) * sizeof(char));
    comm = strcpy(comm, raw_comm);

    struct comm_token *root = NULL;
    struct comm_token *current = NULL;

    while (comm != NULL) {
        char *token = strsep(&comm, ARGS_DELIMITER);
        if (strlen(token) == 0) continue;
        if (root == NULL) {
            root = new_token();
            current = root;
            current->str_token = token;
        } else {
            current = (current->next = new_token());
            current->str_token = token;
        }
    }

    int tokens = 0;
    current = root;
    while (current != NULL) {
        tokens++;
        current = current->next;
    }

    if (tokens == 0) {
        return NULL;
    }

    current = root;
    char **str_tokens = (char **)malloc(tokens * sizeof(char *));
    for (int i = 0; i < tokens; i++) {
        str_tokens[i] = current->str_token;
        struct comm_token *next = current->next;
        free(current);
        current = next;
    }

    return str_tokens;
}

struct comm_token *new_token() {
    return (struct comm_token *)malloc(sizeof(struct comm_token));
}

