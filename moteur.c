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
#include <time.h>

struct Table *array_tables = 0;
int size = 0;

int main(int argc, char **argv)
{
    long clk = CLOCKS_PER_SEC;
    clock_t time1, time2;
    double full_time = 0;

    if (argc != 3)
    {
        fprintf(stderr, "Erreur de format : %s [base.csv] [requetes.sql]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    time1 = clock();
    engine(argv[1], argv[2]);
    time2 = clock();

    full_time += (double) (time2 - time1) / (double) clk;

    printf("\nBenchmark : %lf sec.\n", full_time);

    return EXIT_SUCCESS;
}
