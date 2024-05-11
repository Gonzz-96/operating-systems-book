#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "getchar.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "usage: ./echo <buffered|unbuffered>\n");
        return 1;
    }

    char buf[BUFSIZ];
    int n;

    // when using this version of getchar, stdin
    // gets empty since it's completely read by the
    // buffered_getchar function.
    //
    // Advantages: a single syscall is needed
    // Disadvantages: stding is emptied
    if (strcmp(argv[1], "buffered") == 0) {
        printf("%c\n", (char)buffered_getchar());
    }

    // in this case, the unbuffered version reads one
    // character at a time, leaving the rest to the
    // upcoming while loop
    //
    // Advantages: the buffer is read one char at a time
    // Disadvantages: every call is a system call,
    // requiring the OS to make multiple context switches
    if (strcmp(argv[1], "unbuffered") == 0) {
        printf("%c\n", (char)unbuffered_getchar());
    }

    while ((n = read(0, buf, BUFSIZ)) > 0 ) {
        write(1, buf, n);
    }

    return 0;
}

