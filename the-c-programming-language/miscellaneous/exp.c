#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    char str[50];
    str[0] = '\0';
    for (int i = 0; i < 10; i++) {
        char fn[10];
        sprintf(fn, "%d ", i);
        strcat(str, fn);
    }
    printf("%s", str);
    return 0;
}

