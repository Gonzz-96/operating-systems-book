#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int **parse_input();
int count_blanks(char *);
char *trim(char *);

int main(int argc, char *argv[]) {
    int **nums_arr = parse_input();
}

int **parse_input() {
    size_t n;
    char *input = NULL;
    getline(&input, &n, stdin);
    trim(input);
    int prev_was_blank = 0;
    int count = 0;
    int i = strlen(input);
    while (i-- >= 0) {
        if (isblank(input[i]) ) {
            if (!prev_was_blank) {
                count++;
                prev_was_blank = 1;
            }
        } else {
           prev_was_blank = 0; 
        }
    }
    if (i == 0) {
        return NULL;
    }
    i += 1;
    int **nums = (int **)calloc(i, sizeof(int *));

    size_t begin = 0;
    size_t offset = 0;
    for (size_t i = 0; i < n; i++) {
        if (isblank(input[i])) {
            continue;
        }
        offset++;
    }

   free(input);
   free(nums);

   return NULL; 
}

char *trim(char* str) {
    int len = strlen(str);
    int offset = 0;
    int last_non_blank_idx = 0;
    for (int i = 0; i < len; i++) {
        if (isblank(str[i]))  {
            
        }
    }
}
