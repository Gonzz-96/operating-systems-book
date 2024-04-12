#include <stdio.h>
#include <stdlib.h>

const char* READ_MODE = "r";
const char* WRITE_MODE = "w";

struct Files {
    FILE* source;
    FILE* target;
};

struct Files* open_files(char* source_name, char* target_name);
struct Files* populate_files(struct Files* files,
                             FILE* source,
                             FILE* target);
struct Files* new_files();
int files_ok(struct Files* files);
void close_files(struct Files* files);
void copy_files(struct Files* files);

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        printf("filecopy: usage -> file_copy <source file> <destination file>\n");
        return 1;
    }

    char* source_name = argv[1];
    char* target_name = argv[2];
    struct Files* files = open_files(source_name, target_name);

    switch (files_ok(files)) {
    case 1:
        printf("Target file \"%s\" couldn't be open.\n", target_name);
        return 1;
        break;
    case 2:
        printf("Source file \"%s\" doesn't exist.\n", source_name);
        return 2;
        break;
    }
    
    copy_files(files);
    close_files(files);

    return 0;
}

struct Files* open_files(char* source_name, char* target_name) {
    FILE* source = fopen(source_name, READ_MODE);
    if (source == NULL) {
        return new_files();
    }
    FILE* target = fopen(target_name, WRITE_MODE);
    return populate_files(new_files(), source, target);
}

struct Files* new_files() {
    return (struct Files*)malloc(sizeof(struct Files));
}

struct Files* populate_files(struct Files* files,
                             FILE* source,
                             FILE* target) {
    files->source = source;
    files->target = target;
    return files;
}

int files_ok(struct Files* files) {
    if (files->source == NULL) {
        return 2;
    } else if (files->target == NULL) {
        return 1;
    }
    return 0;
}

void close_files(struct Files* files) {
    fclose(files->source);
    fclose(files->target);
}

void copy_files(struct Files* files) {
    char readchar;
    while ((readchar = fgetc(files->source)) != EOF) {
        fputc(readchar, files->target);
    }
}
