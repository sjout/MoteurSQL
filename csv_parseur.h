#ifndef __CSV_PARSEUR__
#define __CSV_PARSEUR__

/*
Fonction de traitement d'une chaine de caractères du .csv.
*/
char **     csv_parser_line(char const*, const int);

/* 
Fonction de split d'une chaine en fonction d'un délimiteur 
*/                            
char **     split(char *, char);                            

/*
Fonction de libération d'un split.
*/
void        free_split(char **);

#endif
