#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "parser.h"
#include "lexer.h"
#include "tables.h"
#include "requetes.h"
#include "csv_parseur.h"
#include "queue.h"

struct Table *array_tables = 0;
int size = 0;

int main(int argc, char **argv)
{
    FILE *sql = 0;
    tree query = 0;
    joinTree res = 0;
    if (argc != 3)
    {
        fprintf(stderr, "Erreur de format : %s [base.csv] [requetes.sql]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /*  Initialisation des tables */
    set_tables(argv[1]);
    print_tables();
    /*  Initialisation de l'arbre */
    sql = check_open(argv[2]);
    query = AST(sql);
    print_tree(query, 0);

    /*  Initialisation des tables du FROM */
    set_array_from(query->L->next->T->L->T);
    res = init_tree();

    printf("Calculus : %d\n", calculus(query->L->T->L->T->L->T));

    free_tables();
    fclose(sql);

    return EXIT_SUCCESS;
}
