#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "csv_parseur.h"

static const char *const errors[] = 
{
    "Erreur d'allocation mémoire.\n",
    "Erreur de syntaxe.\n",
    "Erreur : Espace mal placé.\n", 
    "Erreur : Utilisation d'un Operateur non correct.\n",
    "Erreur : Partie vide.\n",
    "Erreur : Chaine vide\n"
};

#define MEME_ERR    0
#define SYNT_ERR    1
#define SPACE_ERR   2
#define OP_ERR      3
#define EMPT_ERR    4
#define STR_ERR     5

/*
Fonction de gestion des erreurs.
*/
static void is_error(const int i, const int n)
{
    if (i == SYNT_ERR && n != -1)
    {
        fprintf(stderr, "%sLine in file : %d\n", errors[SYNT_ERR], n);
    }  
    else 
        fprintf(stderr, "%s", errors[i]);
    exit(EXIT_FAILURE);
}

/*
Fonctions de traitement d'une chaine de caractères.
*/
char ** csv_parser_line(const char *str, const int n)
{
    char **split = 0;
    bool in_quotes = FALSE, no_quotes = TRUE, after_comma = FALSE;
    int index = 0, count = 0, lg = 0, pos = 0, space = 0;

    while (str[index] != '\0' && str[index] != '\n' && str[index] != '\r')
    {
        if (str[index] == ',' && !in_quotes)
            count++;
        if (str[index] == '"' && !in_quotes)
            in_quotes = TRUE;
        else if (str[index] == '"' && in_quotes)
            in_quotes = FALSE;
        index++;   
    }
    if (!(split = check_malloc(sizeof(char *) * (count + 2))))
        is_error(MEME_ERR, n);
    split[count + 1] = 0;

    for (index = 0; str[index] != '\0' && str[index] != '\n' && str[index] != '\r';)
    {
        no_quotes = FALSE, lg = 0, after_comma = FALSE;
        while (str[index] != '\0' && str[index] != '\n' && str[index] != '\r' && str[index] != ',')
        {
            space = 0;
            while (str[index] != '\0' && str[index] != '\n' && str[index] != '\r' && (str[index] == ' ' || str[index] == '\t') && !in_quotes)
                index++, space++;
            if (str[index] == '"' && !in_quotes)
                in_quotes = TRUE;
            else if (str[index] == '"' && in_quotes)
                in_quotes = FALSE;

            if (str[index] != '\0' && str[index] != '"' && str[index] != ' ' && str[index] != '\t' && str[index] != '\r' && str[index] != '\n' && !in_quotes && str[index] != ',')
                no_quotes = TRUE;
            if (no_quotes)
                is_error(SYNT_ERR, n);
            if (str[index] != '\0' && str[index] != ',' && str[index] != '\n' && str[index] != '\r') 
                index++, lg++;
        }
        if (lg == 0)
            is_error(SYNT_ERR, n);
        if (!(split[pos] = check_malloc(sizeof(char) * (lg - 1))))
            is_error(MEME_ERR, n);
        split[pos][lg - 2] = '\0';
        strncpy(split[pos], str + index - lg -space + 1, lg - 2);
        pos++;
        if (str[index] == ',') 
            after_comma = TRUE;
        if (str[index] != '\n' && str[index] != '\0' && str[index] != '\r')
            index++;
    }
    if (after_comma)
        is_error(SYNT_ERR, n);

    return split;
}

/* 
Fonction de split d'une chaine en fonction d'un délimiteur                             
*/
char ** split(char *s, char delim)
{
    int count = 0, n = 0, i = 0, l = 0, after_sep = 0; /* nb de séparateurs */
    char **res = 0;
    if(s == 0)
        return 0;

    for(i = 0; s[i] != '\0'; i++)
        if (s[i] == delim)
            count++;

    res = check_malloc((count + 2) * sizeof(char*)); /* allocation du résultat */
    i = 0;
    do{
        l = 0;
        while(s[i] != '\0' && s[i] != delim)
            l++, i++;
        res[n] = check_malloc(sizeof(char) * (l + 1));
        strncpy(res[n], s + i - l, l);
        res[n][l] = '\0';
        n++;
        after_sep = 0;
        if(s[i] == delim)
        {
            i++;
            after_sep = 1;
        }
    } while(s[i] != '\0');
    if(after_sep){ /* sep suivi de fin de chaîne */
        res[n] = check_malloc(sizeof(char));
        res[n][0]='\0';
        n++;
    }
    res[n] = 0;
    return res;
}                           

/*
Fonction de libération d'un split.
*/
void free_split(char **split)
{
    int i = 0;
    
     for (i = 0; split[i] != 0; i++)
        free(split[i]);
    free(split);
}
