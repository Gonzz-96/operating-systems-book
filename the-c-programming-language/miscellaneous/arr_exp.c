#include <stdio.h>
#include <stdlib.h>

int main() {
    // int exp[10];
    int *exp = (int *)calloc(10, sizeof(int));

    for (int i = 0; i < 10; i++) {
        printf("%d ", exp[i]);
    }

    return 0;
}
