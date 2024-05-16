#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int start = atoi(argv[1]);
    int offset = atoi(argv[2]);

    char *name = "Gonz is a superb programmer, dude, I'm being serious";

    char *substr = (char *)malloc(strlen(name) * sizeof(char));

    memcpy(substr, name+start, offset);

    printf("%s\n", substr);

    return  0;
}

