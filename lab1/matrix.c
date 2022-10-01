#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "prototypes.h"

int getint(int* text){ 
    char buf[16];

    do{
        scanf("\n%s",buf);
        if (!isdigit(buf[0]))
            printf("Элементы матрицы - числа. Повторите ввод\n");
} while(!isdigit(buf[0]));
    return atoi(buf);
}



void fill_in_matrix(int** matrix, int rows, int* n)
{
    int i,j,x;


    int elements;

    for(i = 0; i < rows; i++ )
    {
        printf("Введите число элементов в строке %d\n", i);

        scanf("%d", &elements);

        matrix[i] = (int*)malloc(elements*sizeof(int));

        n[i] = elements;
        for(j = 0; j < elements; j++)
        {
            printf("Введите элемент [%d][%d] ", i,j);
            matrix[i][j] = getint(&x);
        }
        
    }
}

void print_matrix(int** matrix, int rows, int* n){  // функция печати матрицы
    int i,j;
    for (i = 0; i < rows; i++){
        for(j = 0; j < n[i]; j++){
            printf("\t%d", matrix[i][j] );
        }
        printf("\n");
    }
}

void swap_min_max(int** matrix, int rows, int* n) // функция, меняющая местами в каждой строке минимум и максимум
{
    int min, max;
    int x = 0, y = 0, t = 0;
    for (int i = 0; i < rows; i++)
    {
        max = -1000000;
        min = 1000000;
            for (int j = 0; j < n[i]; j++)
            {
                if(matrix[i][j] > max)
                {
                    x = j;
                    max = matrix[i][j];
                }
                if(matrix[i][j] < min)
                {
                    y = j;
                    min = matrix[i][j];
                }
            }
        t = matrix[i][y];
        matrix [i][y] = matrix [i][x];
        matrix[i][x] = t;
        }
    
}
