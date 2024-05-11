#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

enum sud_component {
    ROW,
    COLUMN
};

struct args_t {
    int columns_ok;
    int rows_ok;
    int quadrants_ok;
    int **sudoku;
};

struct args_component_t {
    struct args_t *args;
    enum sud_component component;
};

int **read_sudoku();
void print_sudoku(int **);

void *check         (void *);
void *check_quadrant(void *);

pthread_t *create_thread(void *(*)(void *), struct args_component_t *);

int main(int argc, char *argv[]) {

    int **sudoku = read_sudoku();

    struct args_t args;
    struct args_component_t row_args;
    struct args_component_t col_args;
    args.sudoku = sudoku;
    row_args.args      = &args;
    col_args.args      = &args;
    row_args.component = ROW;
    col_args.component = COLUMN;

    pthread_t *rows_thread = create_thread(check, &row_args);
    pthread_t *cols_thread = create_thread(check, &col_args);

    pthread_join(*rows_thread, NULL);
    pthread_join(*cols_thread, NULL);

    printf("For the next sudoku board:\n\n");
    print_sudoku(sudoku);
    printf("rows: %d\n", args.rows_ok);
    printf("columns: %d\n", args.columns_ok);

    return 0;
}

int **read_sudoku() {
    char *num = NULL;
    size_t n;
    int **nums = (int **)malloc(9 * sizeof(int *));
    for (int i = 0; i < 9; i++) {
        nums[i] = (int *)malloc(9 * sizeof(int));
        for (int j = 0; j < 9; j++) {
            getline(&num, &n, stdin);
            nums[i][j] = atoi(num);
            free(num);
        }
    }
    return nums;
}

pthread_t *create_thread(void *(*f)(void *), struct args_component_t *args) {
    pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(tid, &attr, f, args);
    return tid;
}

void *check(void *arg) {
    struct args_component_t *args_component = (struct args_component_t *)arg;
    struct args_t *args = args_component->args;
    enum sud_component component = args_component->component;
    int **sudoku = args->sudoku;
    for (int i = 0; i < 9; i++) {
        int *row_map = (int *)calloc(9, sizeof(int));
        for (int j = 0; j < 9; j++) {
            int column;
            int row;
            switch (args_component->component) {
                case ROW:
                    column = i;
                    row    = j;
                    break;
                default:
                    column = j;
                    row    = i;
                    break;
            }
            int n = sudoku[column][row];
            if (row_map[n] == 0) {
                row_map[n] = 1;
            } else {
                *(component == ROW
                    ? &(args->rows_ok)
                    : &(args->columns_ok)) = -1;
                free(row_map);
                pthread_exit(0);
            }
        }
        free(row_map);
    }
    *(component == ROW
        ? &(args->rows_ok)
        : &(args->columns_ok)) = 1;
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
    printf("\n");
}

