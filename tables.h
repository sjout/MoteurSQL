#ifndef __TABLES__
#define __TABLES__

#include "util.h"

struct Data;                /* Structure representant une Donnée. */
struct Column;              /* Structure d'une colonne */
struct Table;               /* Structure representant une Table. */

struct Data
{
    char                    *str;           /* Valeur d'une donnée de type String. */
    float                   value;          /* Valeur d'une donnée de type Int. */
};

struct Column
{
    char *                  name;
    bool                    is_string;
};

struct Table
{
    char *                  name;           /* Nom d'une table. */
    struct Column *         columns;        /* Nom des colonnes. */
    struct Data *           content;        /* Tableau de donnée en uni-dimensionnel. */
    short int               length, width;  /* Dimension d'une table. */
};

/*
Fonction de séparation d'une chaine de caractères/
*/
char**      str_split(const char *, const char);

/*
Fonctions de chargement des tables.
*/
void        set_name(const char *const, const int, const int);
void        set_columns(const char *const, const int, const int);
void        set_data(const char *const,const int, const int, const int);

/*
Fonction de génération des tables.
*/
void        set_tables(const char *const);

/*
Fonction d'affichage des tables.
*/
void        print_tables();

/*
Fonction de libération de mémoire.
*/
void        free_tables();

#endif
