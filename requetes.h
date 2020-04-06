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

struct pos
{
    struct Table *T;
    int           index;
};

struct select
{
    struct Table *T;
    int           col;
};

conditionJoin   ConditionJoin();
conditionSelect ConditionSelect();
nodeTree        NodeTree(int, conditionJoin, conditionSelect, struct Table *);
joinTree        JoinTree(nodeTree, joinTree, joinTree, joinTree);

void            add_conditionJoin(conditionJoin, tree);
void            add_conditionSelect(conditionSelect, tree);

struct Table *  getTable(const char *);
struct Table *  getTable_from(const char *);
int             getColumn(struct Table *, char *);

void            set_array_from(tree);
joinTree        init_tree();
void            set_tree(tree, joinTree);
void            is_id_in(tree, const char *, tree *, int *);
void            get_id(tree, tree *);
void            count_id(tree, int *_ID);
int             calculus(tree, int);

bool            check_select(conditionSelect, int, struct Table *);
void            engine(char *, char *);

#endif
