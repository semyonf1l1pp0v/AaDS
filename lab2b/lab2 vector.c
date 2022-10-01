#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "RPN_vector.h"


int main(){
    printf("Введите арифметическое выражение:\n");
    int size = 100;
    Vector *stack = createstack(size);
    if(RPN(&stack) == 0) return 0;
    printf("Результат = %d\n", stack->data[0]);
    return 0;
}

