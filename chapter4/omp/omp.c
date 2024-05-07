#include <omp.h>
#include <stdio.h>

int main() {

    // seguential code
    printf("I'm a sequential region\n");

    // parallel code
    #pragma omp parallel
    {
        printf("I'm a parallel region\n");
    }

    // sequential code

    return 0;
}

