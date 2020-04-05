#ifndef __STACK__
#define __STACK__

typedef struct stack_ *stack;

struct stack_
{
    int     index, size;
    char *  array;
};

stack   Stack();

void    resize(stack);

void    push(stack);
char    pop(stack);

void    free_stack(stack);

#endif