#ifndef __LEXER__
#define __LEXER__

#include "util.h"

# define QUERY      300
# define ID         257
# define STRING 	258
# define INT 	   	259
# define COMMA 		260
# define SEMICOLON 	261
# define LPAREN 	262
# define RPAREN 	263
# define DOT 		264
# define PLUS 		265
# define MINUS 		266
# define TIMES 		267
# define DIVIDE 	268
# define EQ 		269
# define NEQ 		270
# define LT 		271
# define LE 		272
# define GT 		273
# define GE 		274
# define AND 		275
# define OR 		276
# define MAX		277
# define MIN		278
# define AVG		279
# define COUNT		280
# define SUM		281
# define SEL		282
# define WHERE		283
# define FROM	    284
# define EXPR_SEL   285
# define EXPR_FROM  286
# define EXPR_WHERE 287
# define EXPR_LEFT  288
# define EXPR_RIGHT 289
# define RENAME     290

/*  Représentation d'un token qui correspond à chaque mot acceptable par le langage */
struct token_
{
    int type;
    union
    {
        int     val;
        string  str;
    } u;
};
typedef struct token_ *token;

/*  Représentation d'un élement de la file qui va correspondre à un Token */
struct node_
{
    token           T;
    struct node_ *  next;
};

struct queue_
{
    struct node_ *  first;
};
typedef struct node_ *  node;
typedef struct queue_ * queue;

/* Constructeur */
node    Node(token, node);
queue   Queue(node);
token   Token(int, int, string);

/* Ajout et Retrait d'un élement de la file */
void    enqueue(queue, token);
token   dequeue(queue);

/* Vérifie si un char est un delimiter */
int     is_delim(int);

/* Gestion de l'automate */
void    set_edges(int, char, char, int); /* Mise en place des relations entre les états */
void    set_automata();                  /* Mise en place de l'automate */

/*  Lexer */
queue   lexer(FILE *);                   /* Retourne une file de token */
void    free_lexer(queue);               /* Libération de la mémoire */

string  id_to_str(int);                  /* Conversion d'un id (int) en string */

#endif
