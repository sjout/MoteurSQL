#ifndef __UTIL__
#define __UTIL__

#include <stdio.h>

#define TRUE 	1
#define FALSE 	0

#ifndef bool
typedef char 	bool;
#endif
typedef char*	string;

string		String(const char *);

/*
 *  Allocation de mémoire et Ouverture de fichier vérifiés.
 */
void *		check_malloc(int);
void *		check_realloc(void *, int);
FILE *      check_open(const char *);

/*
 *  Gestion des erreurs.
 */

#define S_EMPTY_PAREN_ERR 0
#define S_PAREN_ERR       1
#define S_COL_ERR         2
#define S_ID_ERR          3
#define S_OP_ERR          4
#define S_OPERANDE_ERR    5
#define S_NOT_GRAM_ERR    6

#define F_ID_ERR          0
#define F_RENAME_ERR      1
#define F_NOT_GRAM_ERR    2
#define F_NOT_FULL        3

#define W_EMPTY_PAREN_ERR 0
#define W_PAREN_ERR       1
#define W_COL_ERR         2
#define W_ID_ERR          3
#define W_OP_ERR          4
#define W_OPERANDE_ERR    5
#define W_NOT_GRAM_ERR    6
#define W_STR_FAIL        7
#define W_NO_OP           8
#define W_NO_LOPERANDE    9
#define W_NO_ROPERANDE    10
#define W_NO_AND_SEMICOL  11

#define Q_NO_SEL          0
#define Q_NO_ARGS_SEL     1
#define Q_NO_ARGS_FROM    2
#define Q_NO_ARGS_WHERE   3
#define Q_NO_AFTER_WHERE  4
#define Q_NO_SEMICOL      5
#define Q_EMPTY           6
#define Q_NO_FROM         7
#define Q_NO_WHERE        8

#define NOT_FOUND_TABLE   0
#define NOT_FOUND_COL     1
#define IS_STRANGE        2
#define TYPE_ERR          3

void        syntax_error(const char *);
void        lexical_error_select(int);
void        lexical_error_from(int);
void        lexical_error_where(int);
void        lexical_error_query(int);
void        semantic_error(int, const char *);

#endif
