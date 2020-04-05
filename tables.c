#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tables.h"
#include "csv_parseur.h"
#include "queue.h"

extern struct Table *array_tables;
extern int size;

static const char *const errors[] = 
{
    "Erreur d'allocation mémoire.\n",
    "Erreur ouverture du fichier.\n",
    "Erreur de syntaxe.\n",
    "Erreur ouverture du fichier.\n"
};

#define MEM_ERR     0
#define OPEN_ERR    1
#define SYNT_ERR    2
#define FILE_ERR    3

#define MAX_STR     1024

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
Fonctions de chargement des tables.
*/
void set_name(const char *const str, const int pos, const int n)
{
    char **split = csv_parser_line(str, n);
    int i;

    for (i = 0; split[i] != 0; i++);
    if (i > 1)
        is_error(SYNT_ERR, n);
    
    if (!(array_tables[pos].name = (char *) malloc(sizeof(char) * (strlen(split[0]) + 1))))
        is_error(MEM_ERR, n);
    strcpy(array_tables[pos].name, split[0]);
    for (i = 0; split[i] != 0; i++)
        free(split[i]);
    free(split);
}
void set_columns(const char *const str, const int pos, const int n)
{
    char **split = csv_parser_line(str, n);
    int i = 0, j = 0, count = 0, save = 0; 
    for (i = 0; split[i] != 0;i++);
    
    array_tables[pos].width = i;
    if (!(array_tables[pos].columns = (struct Column *) malloc(sizeof(struct Column) * i)))
        is_error(MEM_ERR, n);

    for (i = 0; split[i] != 0; i++)
    {
        count = 0;
        for (j = 0; split[i][j] != '\0'; j++)
            if (split[i][j] == ':')
                count++, save = j;
        if (count != 1)
            is_error(SYNT_ERR, n);

        count = 0;
        for (j = 0;j < save;j++)
        {
            if ((split[i][j] == ' ' || split[i][j] == '\t') && split[i][j + 1] != ' ' && split[i][j] !='\t' && split[i][j + 1] != ':')
                is_error(SYNT_ERR, n);
            
            if (split[i][j] != ' ' && split[i][j] != '\t')
                count++;
        }
        if (count == 0)
            is_error(SYNT_ERR, n);

        if (!(array_tables[pos].columns[i].name = (char *) malloc(sizeof(char) * (count + 1))))
            is_error(MEM_ERR, n);
        
        array_tables[pos].columns[i].name[count] = '\0';
        strncpy(array_tables[pos].columns[i].name, split[i], count);

        count = 0;
        for (j = save + 1; split[i][j] != '\0'; j++)
        {
            if (split[i][j] != ' ' && split[i][j] != '\t' && (split[i][j + 1] == ' ' || split[i][j] == '\t'))
                is_error(SYNT_ERR, n);
            if (split[i][j] != ' ' && split[i][j] != '\t')
                count++;
        }
        if (count == 0)
            is_error(SYNT_ERR, n);

        if (!(strcmp("string", split[i] + j - count)))
        {
            array_tables[pos].columns[i].is_string = true;
        }
        else if (!(strcmp("int", split[i] + j - count)))
        {
            array_tables[pos].columns[i].is_string = false;
        }
        else 
            is_error(SYNT_ERR, n);
    }

    for (i = 0; split[i] != 0; i++)
        free(split[i]);
    free(split);
}
void set_data(const char *const str, const int pos, const int index, const int n)
{
    char **split = csv_parser_line(str, n);
    int i = 0, j = 0, k = 0, tmp = 0;

    for (i = 0; split[i] != 0; i++);
    if (i != array_tables[pos].width)
        printf("2\n"), is_error(SYNT_ERR, n);

    for (i = 0; split[i] != 0; i++)
    {
        if (array_tables[pos].columns[i].is_string)
        {
            if (!(array_tables[pos].content[index * array_tables[pos].width + i].str = (char *) malloc(sizeof(char) * (strlen(split[i]) + 1))))
                is_error(MEM_ERR, n);
            strcpy(array_tables[pos].content[index * array_tables[pos].width + i].str, split[i]);
        }
        else
        {
            for (j = 0; j < strlen(split[i]);j++)
            {
                if ((split[i][j] < '0' || split[i][j] > '9') && split[i][j] != ' ' && split[i][j] != '\t')
                    printf("3\n"), is_error(SYNT_ERR, n);
                if (split[i][j] == ' ')
                {
                    tmp = j;
                    while (split[i][j] != '\0' && (split[i][j] == ' ' || split[i][j] == '\t'))
                        j++;
                    for (k = j; split[i][k] != 0; k++)
                        split[i][tmp++] = split[i][k];
                    split[i][j] = '\0';
                }
            }
               
            array_tables[pos].content[index * array_tables[pos].width + i].value = atoi(split[i]);
        }
    }
    for (i = 0; split[i] != 0; i++)
        free(split[i]);
    free(split);
}

/*
Fonction de génération des tables.
*/

void set_tables(const char *const path)
{
    FILE *file;
    char buff[MAX_STR] = {0};
    char *str = 0;
    struct Queue *Q = 0;
    int *table_lenght = 0;
    int count = 0, ind = 0, pos = 0, k = 0;

    if (!(file = fopen(path, "r")))
        is_error(FILE_ERR, -1);
    if (!(Q = (struct Queue *) malloc(sizeof(struct Queue))))
        is_error(MEM_ERR, -1);
    init_queue(Q);
    if (!(table_lenght = (int *) malloc(sizeof(int))))
        is_error(MEM_ERR, -1);
    table_lenght[0] = 0;
    while (fgets(buff, MAX_STR, file))
    {
        if (buff[0] == '~')
        {
            if (count == 0)
                is_error(SYNT_ERR, -1);
            table_lenght[ind++] = count;
            table_lenght = realloc(table_lenght, (ind + 1) * sizeof(int));
            table_lenght[ind] = 0;
            count = 0, size++;
        }
        else
        {
            add_queue(Q, buff);
            count++;
        }
    }
    if (size == 0)
        is_error(SYNT_ERR, -1);
    if (!(array_tables = (struct Table *) malloc(sizeof(struct Table) * size)))
        is_error(MEM_ERR, 0);
    for (ind = 0; ind < size; ind++)
    {
        array_tables[ind].length = table_lenght[ind] - 2;
    }
    count = 1, ind = 0;
    if (Q->first == 0)
        is_error(SYNT_ERR, -1);

    while (Q->first != 0)
    {
        if(!(str = remove_queue(Q)))
            is_error(SYNT_ERR, -1);
        set_name(str, ind, count);
        free(str);
        count++;
        
        if (Q->first == 0)
            is_error(SYNT_ERR, count);

        if(!(str = remove_queue(Q)))
            is_error(SYNT_ERR, -1);
        set_columns(str, ind, count);
        count++;

        if (Q->first == 0)
            is_error(SYNT_ERR, count);

        if (!(array_tables[ind].content = (struct Data *) malloc(sizeof(struct Data) * array_tables[ind].width * array_tables[ind].length)))
            is_error(MEM_ERR, count);

        pos = 0;
        do
        {
            free(str);
            str = remove_queue(Q);
            set_data(str, ind, pos, count);
            count++, pos++;
        } while (Q->first != 0 && pos < table_lenght[ind] - 2);
	count++;
        free(str);
        ind++;
    }
    fclose(file);
    free(Q), free(table_lenght);
}

/*
Fonction d'affichage des tables.
*/
void print_tables()
{
    int i, j, k, l;

    for (i = 0;i < size; i++)
    {
        printf("\nTable : %s\n\n", array_tables[i].name);
        for (j = 0; j < array_tables[i].width; j++)
            printf("%15s", array_tables[i].columns[j].name);
        printf("\n\n");
        for (j = 0; j < array_tables[i].length; j++)
        {
            for (k = 0; k < array_tables[i].width;k++)
            {
                if (array_tables[i].columns[k].is_string)
                    printf("%15s", array_tables[i].content[j * array_tables[i].width + k].str);
                else
                    printf("%15d", array_tables[i].content[j * array_tables[i].width + k].value);
            }
            printf("\n\n");
        }
	for (l = 0; l < array_tables[i].width * 15; l++)
		printf("+");
	printf("\n\n");
    }
}

/*
Fonction de libération de mémoire.
*/
void free_tables()
{
   int i, j, k;


   for (i = 0;i < size; i++)
   {
        free(array_tables[i].name);
        for (j = 0; j < array_tables[i].length; j++)
        {
            for (k = 0; k < array_tables[i].width; k++)
            {
                if (array_tables[i].columns[k].is_string)
                {
                    free(array_tables[i].content[array_tables[i].width * j + k].str);
                }
            }
        }
        free(array_tables[i].content);

        for (j = 0; j < array_tables[i].width; j++)
            free(array_tables[i].columns[j].name);
        free(array_tables[i].columns);
   }
   free(array_tables);
}
