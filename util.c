#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "util.h"

static char *error_select[] =
{
    "S_Erreur : Parenthèse vide.\n",
    "S_Erreur : Mauvais jeu de parenthèses.\n",
    "S_Erreur : Une expression du SELECT ne peut contenir plus d'une colonne.\n",
    "S_Erreur : Mauvais format -> Table.Colonne.\n",
    "S_Erreur : Manque d'un opérateur.\n",
    "S_Erreur : Manque d'un opérande.\n",
    "S_Erreur : Non inclus dans la grammaire du SELECT.\n"
};
static char *error_from[] =
{
    "F_Erreur : Une expression du FROM ne peut contenir plus d'une colonne.\n",
    "F_Erreur : Mauvais format -> Table Rename.\n",
    "F_Erreur : Non inclus dans la grammaire du FROM.\n"
};
static char *error_where[] =
{
    "W_Erreur : Parenthèse vide.\n",
    "W_Erreur : Mauvais jeu de parenthèses.\n",
    "W_Erreur : Une expression du WHERE ne peut contenir plus d'une colonne.\n",
    "W_Erreur : Mauvais format -> Table.Colonne.\n",
    "W_Erreur : Manque d'un opérateur.\n",
    "W_Erreur : Manque d'un opérande.\n",
    "W_Erreur : Non inclus dans la grammaire du WHERE.\n",
    "W_Erreur : Chaine de caractères mal placé dans le WHERE.\n",
    "W_Erreur : Manque d'un opérateur.\n",
    "W_Erreur : Manque d'un operande gauche.\n",
    "W_Erreur : Manque d'un operande droit.\n",
    "W_Erreur : Manque d'un operateur de jointure ou d'un ';'.\n"
};
static char *error_query[] =
{
    "Q_Erreur : Manque d'un SELECT.\n",
    "Q_Erreur : Besoin d'arguments pour le SELECT.\n",
    "Q_Erreur : Besoin d'arguments pour le FROM.\n",
    "Q_Erreur : Besoin d'arguments pour le WHERE.\n",
    "Q_Erreur : Aucune implémentation après lE WHERE.\n"
    "Q_Erreur : Manque du ';' de fin.\n",
    "Q_Erreur : Requête vide.\n",
    "Q_Erreur : Manque d'un FROM.\n",
    "Q_Erreur : Manque d'un WHERE.\n"
};

void *check_malloc(int len)
{
	void *ptr = malloc(len);
	assert(ptr);

	return ptr;
}
void *check_realloc(void *ptr, int size)
{
	ptr = realloc(ptr, size);
	assert(ptr);

	return ptr;
}
FILE *check_open(const char *path)
{
	FILE *ptr = fopen(path, "r");
	assert(ptr);

	return ptr;
}

string String(const char *str)
{
	string S = check_malloc(sizeof(char) * (strlen(str) + 1));
	strcpy(S, str);

	return S;
}

void syntax_error(const char *str)
{
    fprintf(stderr, "Erreur syntaxique : %s\n", str);
    exit(EXIT_FAILURE);
}
void lexical_error_select(int x)
{
    fprintf(stderr, "%s", error_select[x]);
    exit(EXIT_FAILURE);
}
void lexical_error_from(int x)
{
    fprintf(stderr, "%s", error_from[x]);
    exit(EXIT_FAILURE);
}
void lexical_error_query(int x)
{
    fprintf(stderr, "%s", error_query[x]);
    exit(EXIT_FAILURE);
}
void lexical_error_where(int x)
{
    fprintf(stderr, "%s", error_where[x]);
    exit(EXIT_FAILURE);
}
