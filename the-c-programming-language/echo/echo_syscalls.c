#include <stdio.h>
#include <unistd.h>

#include "getchar.h"

int main() {

    char buf[BUFSIZ];
    int n;

    // when using this version of getchar, stdin
    // gets empty since it's completely read by the
    // buffered_getchar function.
    printf("%c\n", (char)buffered_getchar());

    while ((n = read(0, buf, BUFSIZ)) > 0 ) {
        write(1, buf, n);
    }

    return 0;
}

