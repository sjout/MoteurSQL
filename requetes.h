#ifndef __REQUETES__
#define __REQUETES__

#include "parser.h"

#define COND_JOIN   500
#define TABLE_SEL   501

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
            struct table * T;
        } CS;
    } u;  
};

struct joinTree_
{
    nodeTree val;
    joinTree fg, fd;
};

/*
 * Constructeur
 */
conditionJoin   ConditionJoin();
conditionSelect ConditionSelect();
nodeTree        NodeTree(int, conditionJoin, conditionSelect, struct table *);
joinTree        JoinTree(nodeTree, joinTree, joinTree);

/*
 * Méthodes
 */
void            add_conditionJoin(conditionJoin, tree);
void            add_conditionSelect(conditionSelect, tree);



#endif 
