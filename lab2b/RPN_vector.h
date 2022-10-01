#ifndef RPN_vector_h
#define RPN_vector_h

typedef struct Vector{
    int size;
    int index;
    int *data;
}Vector;

void clear_vector(Vector **stack);

Vector* createstack(int size);

void push_vector(Vector **stack, int value);

int pop_vector(Vector **stack);

int check_vector(char s[], int *j, char str[]);

int RPN (Vector **stack);

#endif /* RPN_vector_h */
