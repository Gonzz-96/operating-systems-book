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
void check_exit_command_or_exit(char **);
void check_previous_command(char **, char ***);
struct command *tokenize(char *);
struct comm_token *new_token();
void check_background_request(int *, struct command *);

struct command {
    char **tokens;
    int num_tokens;
};

int main() {
    shell_loop(); 
    return 0;
}

void shell_loop() {
    int should_run = 1;
    int should_wait_child = 1;
    struct command *tokenized_command = NULL;
    struct command *previous_command  = NULL;
    while (should_run) {
        printf("gsh > ");
        fflush(stdout);

        char *comm = read_comm();
        should_wait_child = 1;
        tokenized_command = tokenize(comm);
        if (tokenized_command == NULL) {
            continue;
        }
        check_exit_command_or_exit(tokenized_command->tokens);
        if (previous_command != NULL) {
            check_previous_command(previous_command->tokens, &(tokenized_command->tokens));
        }
        check_background_request(&should_wait_child, tokenized_command);

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed");
            exit(1);
        } else if (pid == 0) { // child process
            char **tokens = tokenized_command->tokens;
            execvp(tokens[0], tokens);
            exit(errno);
        } else {
            if (should_wait_child) {
                int exit_status;
                wait(&exit_status);
                if (exit_status != 0) {
                    fprintf(stderr, "gsh: command not found: %s\n", tokenized_command->tokens[0]);
                }
            } else {
                printf("Process: [%d] in background\n", pid);
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

struct command *tokenize(char *raw_comm) {
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

    struct command *tokenized_command = (struct command *)malloc(sizeof(struct command));
    tokenized_command->tokens = str_tokens;
    tokenized_command->num_tokens  =  tokens;

    return tokenized_command;
}

struct comm_token *new_token() {
    return (struct comm_token *)malloc(sizeof(struct comm_token));
}

void check_exit_command_or_exit(char **command) {
    if (!strcmp(command[0], "exit")) {
        printf("Bye bye :)\n");
        exit(0);
    }
}

void check_previous_command(char **previous_comm, char ***current_comm) {
    if (!strcmp((*current_comm)[0], "!!")) {
        *current_comm = previous_comm;
    }
}

void check_background_request(int *should_wait_child, struct command *command) {
    char *last_arg = command->tokens[command->num_tokens-1];
    if (strlen(last_arg) == 1 && !strcmp(last_arg, "&")) {
        command->tokens[command->num_tokens-1] = NULL;
        *should_wait_child = 0;
    } else if (last_arg[strlen(last_arg)-1] == '&') {
        last_arg[strlen(last_arg)-1] = '\0';
        *should_wait_child = 0;
    }
}   
