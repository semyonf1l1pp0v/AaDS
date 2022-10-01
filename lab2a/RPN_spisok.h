#ifndef RPN_spisok_h
#define RPN_spisok_h

typedef struct stack{
    int data;
    struct stack *next;
} stack;

void push (stack **head, int data);

int pop (stack **head);

void clearstack(stack **head);

int check(char s[], int *j, char str[]);

int RPN (stack **head);

#endif /* RPN_spisok_h */

