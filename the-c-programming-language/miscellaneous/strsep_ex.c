#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct token {
    char *str;
    struct token *next;
};

struct token *new();
int free_list(struct token *);

int main(int argc, char *argv[]) {

    char *hc_str = (char *)malloc(100 * sizeof(char));
    char *backupptr = hc_str;
    strcpy(hc_str, "Hello     world my name is Gonz");

    struct token *root    = NULL;
    struct token *current = NULL;

    while (hc_str != NULL) {
        // It seems that stack-allocated char pointers (hardcoded strings)
        // cannot be modified in runtime. New memory needs to be allocated
        // in order to update the pointer by any other code 
        char *token = strsep(&hc_str, " ");
        if ((int)token[0] == 0) continue;
        if (root == NULL) {
            root = new(); 
            current = root;
            current->str = token;
        } else {
            current = (current->next = new());
            current->str = token;
        }
    }

    current = root;
    while (current != NULL) {
        printf("Token: %s\n", current->str);
        current = current->next;
    }

    free(backupptr);

    return free_list(root);
}

int free_list(struct token *root) {
    struct token *next = root->next;
    while (root != NULL) {
        free(root);
        root = next;
        if (next != NULL) {
            next = next->next;
        }
    }
    return 0;
}

struct token *new() {
    return (struct token *)malloc(sizeof(struct token));
}

