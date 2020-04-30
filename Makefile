all :
	gcc util.c semantic.c stack.c lexer.c parser.c csv_parseur.c queue.c tables.c requetes.c moteur.c -lncurses -O3 -o Moteur

