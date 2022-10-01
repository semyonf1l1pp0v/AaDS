#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "prototypes.h"


int main(){
    int m = 0;
    int** matrix;
    int* n;
    
    printf("Введите количество строк матрицы: ");
    scanf("%d",&m);
    while (m <= 0){
        printf("Количество строк матрицы - положительное число.\nПовторите ввод\n");
        scanf("%d", &m);
    }
    
    matrix = (int**)malloc(m*sizeof(int*)); // выделяем память под матрицу
    n = (int*)malloc(m*sizeof(int));
    fill_in_matrix (matrix, m, n);
    printf("Исходная матрица:\n");
    print_matrix(matrix, m, n);
    swap_min_max(matrix, m, n);
    printf("\n");
    printf("Изменённая матрица:\n");
    print_matrix(matrix, m, n);
   
    free(matrix); // фукнция освобождения памяти
    free(n);
    return 0;
}

// Пример. Исходная:
// 3 2 6 8 7
// 5 3 7 6
// 4 3 2 9 7 8
//
// Изменённая:
// 3 8 6 2 7
// 5 7 3 6
// 4 3 9 2 7 8
