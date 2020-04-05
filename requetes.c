#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "parser.h"
#include "requetes.h"

extern struct Table *array_tables;
extern int size;


conditionJoin ConditionJoin()
{
    conditionJoin New = check_malloc(sizeof(struct conditionJoin_));
    New->T = check_malloc(sizeof(tree) * 7);
    New->size = 7, New->count_element = 0;

    return New;
}
conditionSelect ConditionSelect()
{
    conditionSelect New = check_malloc(sizeof(struct conditionSelect_));
    New->T = check_malloc(sizeof(tree) * 7);
    New->size = 7, New->count_element = 0;

    return New;
}
nodeTree NodeTree(int type,conditionJoin cj,conditionSelect cs,struct table * t)
{
    nodeTree New = check_malloc(sizeof(struct nodeTree_));
    New->type = type;

    if(type == COND_JOIN)
        New->u.CJ = cj;
    else
    {
        New->u.CS.C = cs;
        New->u.CS.T = t;
    }
}
joinTree JoinTree(nodeTree val, joinTree fg, joinTree fd)
{
    joinTree New = check_malloc(sizeof(struct joinTree_));

    New->val = val;
    New->fg = fg, New->fd = fd;

    return New;
}

void add_conditionJoin(conditionJoin J, tree T)
{
    if(J->count_element = J->size)
    {
        size *= 2;
        check_realloc(J->T, sizeof(struct conditionJoin_) * size);
    }
    J->T[J->count_element++] = T;
}
void add_conditionSelect(conditionSelect S,tree T)
{
    if(S->count_element = S->size)
    {
        size *= 2;
        check_realloc(S->T, sizeof(struct conditionSelect_) * size);
    }
    S->T[S->count_element++] = T;
}
