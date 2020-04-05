#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tables.h"
#include "util.h"
#include "parser.h"
#include "requetes.h"

static struct Table *array_from = 0;
static int           size_from  = 0;

extern struct Table *array_tables;
extern int           size;

conditionJoin ConditionJoin()
{
    conditionJoin New = check_malloc(sizeof(struct conditionJoin_));

    New->size = 7, New->count_element = 0;
    New->T = check_malloc(sizeof(tree) * New->size);

    return New;
}
conditionSelect ConditionSelect()
{
    conditionSelect New = check_malloc(sizeof(struct conditionSelect_));

    New->size = 7, New->count_element = 0;
    New->T = check_malloc(sizeof(tree) * New->size);

    return New;
}
nodeTree NodeTree(int type, conditionJoin Cjoin, conditionSelect Csel, struct Table *T)
{
    nodeTree New = check_malloc(sizeof(struct nodeTree_));

    New->type = type;
    if (type == COND_JOIN)
        New->u.CJ = Cjoin;
    else if (type == TABLE_SEL)
        New->u.CS.C = Csel, New->u.CS.T = T;
    else if (type != PROD_CARTE)
    {
        fprintf(stderr, "Erreur : Type de condition inconnu.\n");
        exit(EXIT_FAILURE);
    }

    return New;
}
joinTree JoinTree(nodeTree N, joinTree left, joinTree right, joinTree father)
{
    joinTree T = check_malloc(sizeof(struct joinTree_));

    T->val = N;
    T->left = left, T->right = right, T->father = father;

    return T;
}

void add_conditionJoin(conditionJoin Cjoin, tree T)
{
    if (Cjoin->count_element == Cjoin->size)
    {
        Cjoin->size *= 2;
        check_realloc(Cjoin->T, sizeof(tree) * Cjoin->size);
    }
    Cjoin->T[Cjoin->count_element++] = T;
}
void add_conditionSelect(conditionSelect Csel, tree T)
{
    if (Csel->count_element == Csel->size)
    {
        Csel->size *= 2;
        check_realloc(Csel->T, sizeof(tree) * Csel->size);
    }
    Csel->T[Csel->count_element++] = T;
}

struct Table *getTable(const char *name)
{
    int i = 0;

    for (i = 0; i < size; i++)
        if (!strcmp(name, array_tables[i].name))
            return &(array_tables[i]);

    return 0;
}

void set_array_from(tree from)
{
    struct Table *T = 0;
    list li = from->L;
    int i = 0;

    while (li != 0)
    {
        size_from++;
        li = li->next;
    }
    array_from = check_malloc(sizeof(struct Table) * size_from);

    li = from->L;

    while (li != 0 && i < size_from)
    {
        if (li->T->T->type == RENAME)
        {
            T = getTable(li->T->L->T->T->u.str);
            if (T == 0)
            {
                fprintf(stderr, "Erreur : Table -> %s inconnu.\n", li->T->L->T->T->u.str);
                exit(EXIT_FAILURE);
            }
            array_from[i] = *T;
            array_from[i++].name = li->T->L->next->T->T->u.str;
        }
        else
        {
            T = getTable(li->T->T->u.str);
            if (T == 0)
            {
                fprintf(stderr, "Erreur : Table -> %s inconnu.\n", li->T->T->u.str);
                exit(EXIT_FAILURE);
            }
            array_from[i++] = *T;
        }
        li = li->next;
    }
}
joinTree init_tree()
{
    assert(size_from > 0);
    int index = size_from - 1;
    joinTree root = JoinTree(NodeTree(PROD_CARTE, 0, 0, 0), 0, 0, 0), tmp = root;
    nodeTree new = 0;

    for (; index >= 0; index--)
    {
        new = NodeTree(TABLE_SEL, 0, 0, &(array_from[index]));
        if (index != 0)
            tmp->right = JoinTree(new, 0, 0, tmp);
        else
            tmp->left = JoinTree(new, 0, 0, tmp);
        if (index > 1)
        {
            tmp->left = JoinTree(NodeTree(PROD_CARTE, 0, 0, 0), 0, 0, tmp);
            tmp = tmp->left;
        }
    }

    return root;
}
void set_tree(tree T)
{
}
int calculus(tree T)
{
    int left = 0, right = 0;

    if (T == 0)
        return 0;
    if (T->L == 0)
        return T->T->u.val;
    if (T->T->type == DOT)
        return 4;
    left = calculus(T->L->T);
    right= calculus(T->L->next->T);
    if (T->T->type == PLUS)
        return left + right;
    else if (T->T->type == MINUS)
        return left - right;
    else if (T->T->type == TIMES)
        return left * right;
    else if (T->T->type == DIVIDE)
        return left / right;

    return 0;
}
