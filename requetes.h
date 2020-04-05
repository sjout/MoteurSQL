#ifndef __REQUETES__
#define __REQUETES__

#include "parser.h"

#define COND_JOIN   500
#define TABLE_SEL   501
#define PROD_CARTE  502

typedef struct nodeTree_        *   nodeTree;
typedef struct joinTree_        *   joinTree;
typedef struct conditionJoin_   *   conditionJoin;
typedef struct conditionSelect_ *   conditionSelect;

struct conditionJoin_
{
    tree *  T;
    int     size, count_element;
};

struct conditionSelect_
{
    tree *  T;
    int     size, count_element;
};

struct nodeTree_
{
    int     type;
    union
    {
        conditionJoin       CJ;
        struct nodeJoin
        {
            conditionSelect C;
            struct Table *  T;
        } CS;
    } u;
};

struct joinTree_
{
    nodeTree val;
    joinTree left, right, father;
};

conditionJoin   ConditionJoin();
conditionSelect ConditionSelect();
nodeTree        NodeTree(int, conditionJoin, conditionSelect, struct Table *);
joinTree        JoinTree(nodeTree, joinTree, joinTree, joinTree);

void            add_conditionJoin(conditionJoin, tree);
void            add_conditionSelect(conditionSelect, tree);

struct Table *  getTable(const char *);

void            set_array_from(tree);
joinTree        init_tree();
void            set_tree(tree);
int             calculus(tree);

#endif
