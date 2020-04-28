#ifndef __PARSER_SQL__
#define __PARSER_SQL__

#include "lexer.h"
#include "stack.h"

typedef struct list_ *  list;
typedef struct tree_ *  tree;

/* Structure de la liste qui representera les fils d'un noeud de l'arbre */
struct list_
{
    tree            T;
    list            next;
};

/* Representation de la requête avec un arbre */
struct tree_
{
    token           T;
    tree            father;
    list            L;
};

/* Constructeur */
list    List(tree, list);
tree    Tree(token, tree, list);

/* Fonction de gestion de l'arbre et des fils */
list    get_last(list);
list    get_last_and_replace(tree);
void    add_tree(tree, tree);

/* Fonction de verification de la valeur d'un Token */
bool    is_delimiter(int);
bool    is_operator_comp(int);
bool    is_operator_join(int);
bool    is_operator_arith(int);
bool    is_keyword(int);
bool    is_agregate(int);

/* Définition de la grammaire */
void    expr_select(queue, tree, int *, token *);
void    expr_from(queue, tree, int *, token *);
void    expr_where(queue, tree, int *, token *, stack);
void    expr_where_full(queue, tree, token *);
void    query_tree(queue, tree);
void    check_query(tree);

/* Fonction de création de l'arbre de la requête */
tree 	AST(FILE*);

/* Affichage de l'abre */
void 	print_tree(tree, int);

/* Libération mémoire */
void    free_ast(tree);

#endif
