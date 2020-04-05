#ifndef __QUEUE__
#define __QUEUE__

struct Element
{
    char *              str;
    struct Element *    next;
};

struct Queue
{
    struct Element *    first;
};

void        init_queue(struct Queue *);
void        init_element(struct Element *);
void        add_queue(struct Queue *, char *);
char*       remove_queue(struct Queue *);

#endif