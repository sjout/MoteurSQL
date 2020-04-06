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
    if (argc != 3)
    {
        fprintf(stderr, "Erreur de format : %s [base.csv] [requetes.sql]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    engine(argv[1], argv[2]);

    return EXIT_SUCCESS;
}
