#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "RPN_spisok.h"


int main(){
    printf("Введите арифметическое выражение:\n");
    stack *head = NULL;
    if(RPN(&head) == 0) return 0;
    printf("Результат = %d\n", head->data);
    free(head);
    return 0;
}


