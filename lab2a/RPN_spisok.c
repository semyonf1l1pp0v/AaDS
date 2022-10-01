#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "RPN_spisok.h"

void push (stack **head, int data){
    stack *tmp = malloc(sizeof(stack));
    tmp->next = *head;
    tmp->data = data;
    *head = tmp;
}

int pop (stack **head){
    stack *out;
    int data;
    if(*head == NULL){
        printf("Стек пуст\n");
        return 0;
    }
    out = *head;
    *head = (*head)->next;
    data = out->data;
    free(out);
    return data;
}

void clearstack(stack **head){
    stack *out;
    while(*head){
        out = *head;
        *head = (*head)->next;
        free(out);
    }
}

int check(char s[], int *j, char str[]){
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

int RPN (stack **head){
    int k = 0;
    int count = 0, c, j = 0;
    char s[100], str[100];
    while ((str[j] = getchar()) != '\n') {
        j++;
    }
    str[j] = '\n';
    str[++j] = '\0';
    j = 0;
    while((c = check(s, &j, str)) != '\n'){
        if(c == '0'){
            ++count;
            push(head, atoi(s));
        }
    else if (c == '+' || c == '-' || c == '/' || c == '*'){
    switch(c){
            case '+':
                push(head, (pop(head) + pop(head)));
                break;
            case '*':
                push(head, (pop(head) * pop(head)));
                break;
            case '-':
                k = pop(head);
                push(head, (pop(head) - k));
                break;
            case '/':
                k = pop(head);
                if(k == 0){
                    printf("Деление на ноль!\n");
                    clearstack(head);
                    return 0;
                } else {
                    push(head, (pop(head) / k));
                }
            break;
            }
    }
    }
    return 1;
    }

