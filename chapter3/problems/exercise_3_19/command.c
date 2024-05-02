#include <stdlib.h>
#include <string.h>

#include "command.h"

/*
 *  Implementation of command.h
 */

char **commargv(int argc, char *argv[]) {
    char **newargv = (char **)malloc(argc * sizeof(char *));
    for (int i = 0; i < argc - 1; i++) {
        newargv[i] = (char *)malloc(strlen(argv[i+1]) * sizeof(char));
        newargv[i] = argv[i+1];
    }
    // following the convention of the 'exec' family of system calls, the "argv" argument
    // must always start with the name of the program to execute, and end with a 
    // null pointer otherwise, the function will return an error
    newargv[argc-1] = NULL;
    
    return newargv;
}
