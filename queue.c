#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

void init_queue(struct Queue *Q)
{
    assert(Q != 0);
    Q->first = 0;
}

void init_element(struct Element *e)
{
    assert(e != 0);
    e->str = 0, e->next = 0;
}

void add_queue(struct Queue *Q, char *str)
{
    struct Element *e = (struct Element *) malloc(sizeof (struct Element));
    struct Element *tmp = 0;

    init_element(e);

    assert(Q != 0 && str != 0);

    e->str = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(e->str, str);

    if (Q->first == 0)
    {
        Q->first = e;
    }
    else 
    {
        tmp = Q->first;
        while (tmp->next != 0)
            tmp = tmp->next;
        tmp->next = e;
    }
}

char* remove_queue(struct Queue *Q)
{
    struct Element *tmp = 0;
    char *save = 0;
    assert(Q != 0);

    if (Q->first != 0)
    {
        tmp = Q->first->next;
        save = Q->first->str;
        free(Q->first);
        Q->first = tmp;
        return save;
    }
    return 0;
}