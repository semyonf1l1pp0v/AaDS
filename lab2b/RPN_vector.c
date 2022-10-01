#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "RPN_vector.h"

void clear_vector(Vector **stack){
    free((*stack)->data);
    free(*stack);
    *stack = NULL;
}

Vector* createstack(int size){
    Vector *out = NULL;
    out =  malloc(sizeof(Vector));
    out->size = size;
    out->index = 0;
    out->data = malloc(out->size * sizeof(int));
    return out;
}

void push_vector(Vector **stack, int value){
    if((*stack)->index >= (*stack)->size){
        printf("Stack overflow!");
        clear_vector(stack);
    } else {
        (*stack)->data[(*stack)->index] = value;
        (*stack)->index++;
    }
}

int pop_vector(Vector **stack){
    return (*stack)->data[--(*stack)->index];
}

int check_vector(char s[], int *j, char str[]){
    int  i;
    while((s[0] = str[*j]) == ' ' || str[*j] == '\t'){
        (*j)++;
    }
    s[1] = '\0';
    if(!isdigit(s[0])){
        return(str[(*j)++]);
    }
    i = 0;
    if(isdigit(s[0]))
        while(isdigit(s[++i] = str[++(*j)]));
    s[i] = '\0';
    return '0';
}

int RPN (Vector **stack){
    int k = 0;
    int count = 0, c, j = 0;
    char s[100], str[100];
    while ((str[j] = getchar()) != '\n') {
        j++;
    }
    str[j] = '\n';
    str[++j] = '\0';
    j = 0;
    while((c = check_vector(s, &j, str)) != '\n'){
        if(c == '0'){
            ++count;
            push_vector(stack, atoi(s));
        }
        else if (c == '+' || c == '-' || c == '/' || c == '*'){
    switch(c){
        case '+':
            push_vector(stack, (pop_vector(stack) + pop_vector(stack)));
            break;
        case '*':
            push_vector(stack, (pop_vector(stack) * pop_vector(stack)));
            break;
        case '-':
            k = pop_vector(stack);
            push_vector(stack, (pop_vector(stack) - k));
            break;
        case '/':
            k = pop_vector(stack);
            if(k == 0){
                printf("Деление на ноль!\n");
                clear_vector(stack);
                return 0;
            } else {
                push_vector(stack, (pop_vector(stack) / k));
            }
            break;
    }
        }
    }
    return 1;
}
