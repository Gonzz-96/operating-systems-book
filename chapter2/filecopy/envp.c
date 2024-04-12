#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
    
    int i = 0;
    char* arg;
    while((arg = envp[i]) != NULL) {
        printf("%s\n", arg);
        i++;
    }

    return 0;
}
