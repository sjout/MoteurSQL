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

/* Définition des conditions de jointure. */
struct conditionJoin_
{
    tree *  T;                      /* Tableau d'arbre de conditon de jointure */
    int     size, count_element;
};

/* Définition des conditions de selection. */
struct conditionSelect_
{
    tree *  T;                      /* Tableau d'arbre de conditon de selection */
    int     size, count_element;
};

/* Representation d'un noeud de l'arbre de jointure */
struct nodeTree_
{
    int     type;                   /* Le type correspond au macro ci-dessus */
    union                           /* Un noeud peut être une jointure, selection ou un produit cartésien. */
    {
        conditionJoin       CJ;
        struct nodeJoin
        {
            conditionSelect C;
            struct Table *  T;
        } CS;
    } u;
};

/* Representation de l'arbre de jointure */
struct joinTree_
{
    nodeTree val;
    joinTree left, right, father;
};

/*
 *  Representation de la position des indices des boucles imbriqués
 *  lors de la lecture de l'abre de jointure
 */
struct pos
{
    struct Table *T;
    int           index;
};

/* Representation d'une selection */
struct select
{
    struct Table *T;
    int           col;
};

/* Constructeurs */
conditionJoin   ConditionJoin();
conditionSelect ConditionSelect();
nodeTree        NodeTree(int, conditionJoin, conditionSelect, struct Table *);
joinTree        JoinTree(nodeTree, joinTree, joinTree, joinTree);

/* Méthode sur les conditions de jointure/selection */
void            add_conditionJoin(conditionJoin, tree);
void            add_conditionSelect(conditionSelect, tree);

/* Fonction qui retourne les tables/colonnes depuis un nom */
struct Table *  getTable(const char *);
struct Table *  getTable_from(const char *);
int             getColumn(struct Table *, char *);
struct Table *  findTable(const char *);

/* Fonction d'intialisation */
void            set_array_from(tree);
joinTree        init_tree();
void            set_tree(tree, joinTree);
void            is_id_in(tree, const char *, tree *, int *);
void            get_id(tree, tree *);
void            get_dot(tree, tree *);
void            count_id(tree, int *_ID);
int             calculus(tree, int);
void            debug_tree(joinTree *);

/* Verification de la validé des conditions de jointure/selection */
bool            check_select(conditionSelect, int, struct Table *);
bool            check_join(conditionJoin, int, struct Table *, struct pos *, int _len);

/* Libération de la mémoire */
void            free_joinTree(joinTree);

/* Moteur de requête SQL */
void            engine(char *, char *);

#endif
