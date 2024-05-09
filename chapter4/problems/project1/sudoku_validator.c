#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int **read_sudoku();
void print_sudoku(int **);

void *check_quadrant(void *);
void *check_columns (void *);
void *check_rows    (void *);

struct args {
    int columns_ok;
    int rows_ok;
    int quadrants_ok;
    int **sudoku;
};

pthread_t *create_thread(void *(*)(void *), struct args *);

int main(int argc, char *argv[]) {

    int **sudoku = read_sudoku();
    struct args t_args;
    t_args.sudoku = sudoku;
    print_sudoku(t_args.sudoku);

    pthread_t *rows_thread = create_thread(check_rows, &t_args);

    pthread_join(*rows_thread, NULL);

    printf("rows: %d\n", t_args.rows_ok);
    print_sudoku(sudoku);

    return 0;
}

int **read_sudoku() {
    char *num;
    size_t n;
    int **nums = (int **)malloc(9 * sizeof(int *));
    for (int i = 0; i < 9; i++) {
        nums[i] = (int *)malloc(9 * sizeof(int));
        for (int j = 0; j < 9; j++) {
            getline(&num, &n, stdin);
            nums[i][j] = atoi(num);
        }
    }
    return nums;
}

pthread_t *create_thread(void *(*f)(void *), struct args *t_args) {
    pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(tid, &attr, f, t_args);
    return tid;
}

void *check_rows(void *arg) {
    struct args *t_args = (struct args *)arg;
    int **sudoku = t_args->sudoku;
    for (int i = 0; i < 9; i++) {
        int *row_map = (int *)calloc(9, sizeof(int));
        for (int j = 0; j < 9; j++) {
            int n = sudoku[i][j];
            printf("thread: %d\n", n);
            if (row_map[n] == 0) {
                row_map[n] = 1;
            } else {
                t_args->rows_ok = -1;
                free(row_map);
                pthread_exit(0);
            }
        }
        free(row_map);
    }
    t_args->rows_ok = 1;
    pthread_exit(0);
}

void *check_columns(void *arg) {
    int column = *((int *)arg);
    pthread_exit(0);
}

void *check_quadrant(void *arg) {
    pthread_exit(0);
}

void print_sudoku(int **sudoku) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
}
