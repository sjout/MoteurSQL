#ifndef __PARSER_SQL__
#define __PARSER_SQL__

#include "lexer.h"
#include "stack.h"

typedef struct list_ *  list;
typedef struct tree_ *  tree;

struct list_
{

    tree            T;
    list            next;
};

struct tree_
{
    token           T;
    tree            father;
    list            L;
};

list    List(tree, list);
tree    Tree(token, tree, list);

list    get_last(list);
list    get_last_and_replace(tree);
void    add_tree(tree, tree);

bool    is_delimiter(int);
bool    is_operator_comp(int);
bool    is_operator_join(int);
bool    is_operator_arith(int);
bool    is_keyword(int);
bool    is_agregate(int);

void    expr_select(queue, tree, int *, token *);
void    expr_from(queue, tree, int *, token *);
void    expr_where(queue, tree, int *, token *, stack);
void    expr_where_full(queue, tree, token *);
void    query_tree(queue, tree);
void    check_query(tree);
tree 	AST(FILE *);

void 	print_tree(tree, int);

#endif
