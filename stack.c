#include <stdlib.h>
#include <assert.h>
#include "util.h"
#include "stack.h"

stack Stack()
{
    stack S = check_malloc(sizeof(struct stack_));
    
    S->index = 0, S->size = 7;
    S->array = check_malloc(sizeof(char) * 7);

    return S;
}

void resize(stack S)
{
    if (S->size == S->index)
    {
        S->size *= 2;
        S->array = check_realloc(S->array, S->size);
    }
}

void push(stack S)
{
    resize(S);
    S->array[S->index++] = '(';
}
char pop(stack S)
{
    assert(S != 0);
    if (S->index == 0)
        return 0;
    S->index -= 1;

    return S->array[S->index];
}

void free_stack(stack S)
{
    free(S->array);
    free(S);
}
