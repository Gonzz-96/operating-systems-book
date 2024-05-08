#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int **read_sudoku();
void print_sudoku(int **);

int main(int argc, char *argv[]) {

    int **sudoku = read_sudoku();
    print_sudoku(sudoku);
    
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

void print_sudoku(int **sudoku) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
}
