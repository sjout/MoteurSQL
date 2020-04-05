#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "lexer.h"

static int edges[][256] = 
{
	{0},		/*  State 0  */
	{0},		/*  State 1  */
	{0},		/*  State 2  */
	{0},		/*  State 3  */
	{0},		/*  State 4  */
	{0},		/*  State 5  */
	{0},		/*  State 6  */
	{0},		/*  State 7  */
	{0},		/*  State 8  */
	{0},		/*  State 9  */
	{0},		/*  State 10 */
	{0},		/*  State 11 */
	{0},		/*  State 12 */
	{0},		/*  State 13 */
	{0},		/*  State 14 */
	{0},		/*  State 15 */
	{0},		/*  State 16 */
	{0},		/*  State 17 */
	{0},		/*  State 18 */
	{0},		/*  State 19 */
	{0},		/*  State 20 */
	{0},		/*  State 21 */
	{0},		/*  State 22 */
	{0},		/*  State 23 */
	{0},		/*  State 24 */
	{0},		/*  State 25 */
	{0},		/*  State 26 */
	{0},		/*  State 27 */
};

static int action[] = 
{
	-1,		/*  State 0  */
	-1,		/*  State 1  */
	-1,		/*  State 2  */
	-1,		/*  State 3  */
	-1,		/*  State 4  */
	-1,		/*  State 5  */
	INT,	/*  State 6  */
	ID,		/*  State 7  */
	-1,		/*  State 8  */
	-1,		/*  State 9  */
	-1,		/*  State 10 */
	-1,		/*  State 11 */
	SEL,	/*  State 12 */
	-1,		/*  State 13 */
	-1,		/*  State 14 */
	-1,		/*  State 15 */
	WHERE,	/*  State 16 */
	-1,		/*  State 17 */
	-1,		/*  State 18 */
	FROM,	/*  State 19 */
	STRING,	/*  State 20 */	
	-1,		/*  State 21 */
	-1,		/*  State 22 */
	AND,	/*  State 23 */
	PLUS,	/*  State 24 */
	MINUS,	/*  State 25 */
	DIVIDE,	/*  State 26 */
	TIMES,	/*  State 27 */
};

static int delimiters[][2] = 
{
	{'.', DOT},
	{',', COMMA},
	{';', SEMICOLON},
	{'(', LPAREN},
	{')', RPAREN},
	{'=', EQ},
	{'>', GT},
	{'<', LT},
    {'+', PLUS},
    {'-', MINUS},
    {'*', TIMES},
    {'+', DIVIDE}
};

void set_edges(int state, char start, char end, int at)
{
	int i;

	for (i = start; i <= end; i++)
		edges[state][i] = at;
}

void set_automata()
{
	set_edges(1, 'S', 'S', 2);
	set_edges(2, 'E', 'E', 8);
	set_edges(8, 'L', 'L', 9);
	set_edges(9, 'E', 'E', 10);
	set_edges(10, 'C', 'C', 11);
	set_edges(11, 'T', 'T', 12);

	set_edges(1, 'W', 'W', 3);
	set_edges(3, 'H', 'H', 13);
	set_edges(13, 'E', 'E', 14);
	set_edges(14, 'R', 'R', 15);
	set_edges(15, 'E', 'E', 16);

	set_edges(1, 'F', 'F', 4);
	set_edges(4, 'R', 'R', 17);
	set_edges(17, 'O', 'O', 18);
	set_edges(18, 'M', 'M', 19);

	set_edges(1, '"', '"', 5);
	set_edges(5, 'a', 'z', 5);
	set_edges(5, 'A', 'Z', 5);
	set_edges(5, '1', '9', 5);
	set_edges(5, '_', '_', 5);
	set_edges(5, '"', '"', 20);

	set_edges(1, '0', '9', 6);
	set_edges(6, '0', '9', 6);

	set_edges(1, 'a', 'z', 7);
	set_edges(7, 'a', 'z', 7);
	set_edges(7, '1', '9', 7);
	set_edges(7, '_', '_', 7);

	set_edges(1, 'A', 'A', 21);
	set_edges(21, 'N', 'N', 22);
	set_edges(22, 'D', 'D', 23);


	set_edges(1, '+', '+', 24);
	set_edges(1, '-', '-', 25);
	set_edges(1, '/', '/', 26);
	set_edges(1, '*', '*', 27);
}

int is_delim(int x)
{
	int i = 0;

	for (i = 0; i < (sizeof(delimiters) / sizeof(*delimiters)); i++)
		if (delimiters[i][0] == x)
			return delimiters[i][1];

	return -1;
}

token Token(int t, int v, string s)
{
	token T = check_malloc(sizeof(struct token_));

	T->type = t;
	if (t == ID || t == STRING)
		T->u.str = s;
	else
		T->u.val = v;

	return T;
}

node Node(token T, node N)
{
    node New = check_malloc(sizeof(struct node_));

    New->T = T, New->next = N;

    return New;
}

queue Queue(node N)
{
    queue Q = check_malloc(sizeof(struct queue_));

    Q->first = N;

    return Q;
}

void enqueue(queue Q, token T)
{
    assert(Q != 0);
    node Tmp = Q->first;
    node New = Node(T, 0);

    if (Tmp == 0)
        Q->first = New;
    else
    {
        while (Tmp->next != 0)
            Tmp = Tmp->next;
        Tmp->next = New;
    }
}

token dequeue(queue Q)
{
    assert(Q != 0);
    node Tmp = Q->first;
    node N = 0;
    token T = 0;

    if (Q->first == 0)
    {
        free(Q);
        return 0;
    }
    T = Tmp->T;
    Q->first = Tmp->next;
    free(Tmp);

    return T;
}

queue lexer(FILE *file)
{
	queue Q = Queue(0);
	token T = 0;
	int c = 0, tmp = 0, pos = 0, state = 1, index = 0, size = 7;
	bool good = TRUE, free_str = FALSE;
	string str = check_malloc(sizeof(char) * size);
	str[size - 1] = '\0';
	set_automata();
	int val = -1;
	
	while ((c = fgetc(file)) != EOF && good)
	{
		free_str = TRUE;
		if ((val = is_delim(c)) != -1)
		{
			if (index != 0)
			{
				str[index] = '\0';
				if (action[state] == ID || action[state] == STRING)
					free_str = FALSE, T = Token(action[state], 0, str);
				else if (action[state] == INT)
					T = Token(action[state], atoi(str), 0);
				else
					T = Token(action[state], 0, 0);
				enqueue(Q, T);
				if (action[state] == -1)
                    syntax_error(str);
				if (val == GT)
				{
					if ((c = fgetc(file)) != EOF)
					{
						if (c == '=')
							T = Token(GE, GE, 0);
						else
							T = Token(GT, GT, 0);
					}
					else
						T = Token(GT, GT, 0);
				}
				else if (val == LT)
				{
					if ((c = fgetc(file)) != EOF)
					{
						if (c == '=')
							T = Token(GE, GE, 0);
						else
							T = Token(GT, GT, 0);
					}
					else
						T = Token(LT, LT, 0);
				}
                else if (val == PLUS)
                    T = Token(PLUS, PLUS, 0);
                else if (val == MINUS)
                    T = Token(MINUS, MINUS, 0);
                else if (val == TIMES)
                    T = Token(TIMES, TIMES, 0);
                else if (val == DIVIDE)
                    T = Token(DIVIDE, DIVIDE, 0);
				else
					T = Token(val, val, 0);
				enqueue(Q, T);
				index = 0, state = 1, size = 7;
				if (free_str) free(str);
				str = check_malloc(sizeof(char) * size);
				str[size - 1] = '\0';
			}
			else
			{
				T = Token(val, val, 0);
				enqueue(Q, T);
				index = 0, state = 1, size = 7;
				if (free_str) free(str);
				str = check_malloc(sizeof(char) * size);
				str[size - 1] = '\0';
			}
		}
		else if (c == '\n' || c == '\r' || c == '\t' || c == ' ')
		{
			if (index != 0)
			{
				str[index] = '\0';
				if (action[state] == -1)
                    syntax_error(str);
				if (action[state] == ID || action[state] == STRING)
					free_str = FALSE, T = Token(action[state], 0, str);
				else if (action[state] == INT)
					T = Token(action[state], atoi(str), 0);
				else
					T = Token(action[state], 0, 0);
				enqueue(Q, T);
				index = 0, state = 1, size = 7;
				if (free_str) free(str);
				str = check_malloc(sizeof(char) * size);
				str[size - 1] = '\0';
			}
		}
		else
		{
			if (index == size - 1)
			{
				size *= 2;
				str = check_realloc(str, size);
				str[size - 1] = '\0';
			}
			state = edges[state][c];
			str[index++] = c;
		}
	}
    if (Q->first == 0)
        syntax_error("Fichier vide.");
	if (str != 0) free(str);
	return Q;
}

void free_lexer(queue Q)
{
	token T = dequeue(Q);

	while (T != 0)
	{
		if (T != 0 && (T->type == ID || T->type == STRING))
			free(T->u.str);
		free(T);
		T = dequeue(Q);
	}
}

string id_to_str(int x)
{
	string s = 0;
	
	switch (x)
	{
		case 257 :
			s = check_malloc(sizeof(char) * strlen("ID") + 1);
			strcpy(s, "ID");
			break;
		case 258 :
			s = check_malloc(sizeof(char) * strlen("STRING") + 1);
			strcpy(s, "STRING");
			break;
		case 259 :
			s = check_malloc(sizeof(char) * strlen("INT") + 1);
			strcpy(s, "INT");
			break;
		case 260 :
			s = check_malloc(sizeof(char) * strlen("COMMA") + 1);
			strcpy(s, "COMMA");
			break;
		case 261 :
			s = check_malloc(sizeof(char) * strlen("SEMICOLON") + 1);
			strcpy(s, "SEMICOLON");
			break;
		case 262 :
			s = check_malloc(sizeof(char) * strlen("LPAREN") + 1);
			strcpy(s, "LPAREN");
			break;
		case 263 :
			s = check_malloc(sizeof(char) * strlen("RPAREN") + 1);
			strcpy(s, "RPAREN");
			break;
		case 264 :
			s = check_malloc(sizeof(char) * strlen("DOT") + 1);
			strcpy(s, "DOT");
			break;
		case 265 :
			s = check_malloc(sizeof(char) * strlen("PLUS") + 1);
			strcpy(s, "PLUS");
			break;
		case 266 :
			s = check_malloc(sizeof(char) * strlen("MINUS") + 1);
			strcpy(s, "MINUS");
			break;
		case 267 :
			s = check_malloc(sizeof(char) * strlen("TIMES") + 1);
			strcpy(s, "TIMES");
			break;
		case 268 :
			s = check_malloc(sizeof(char) * strlen("DIVIDE") + 1);
			strcpy(s, "DIVIDE");
			break;
		case 269 :
			s = check_malloc(sizeof(char) * strlen("EQ") + 1);
			strcpy(s, "EQ");
			break;
		case 270 :
			s = check_malloc(sizeof(char) * strlen("NEQ") + 1);
			strcpy(s, "NEQ");
			break;
		case 271 :
			s = check_malloc(sizeof(char) * strlen("LT") + 1);
			strcpy(s, "LT");
			break;
		case 272 :
			s = check_malloc(sizeof(char) * strlen("LE") + 1);
			strcpy(s, "LE");
			break;
		case 273 :
			s = check_malloc(sizeof(char) * strlen("GT") + 1);
			strcpy(s, "GT");
			break;
		case 274 :
			s = check_malloc(sizeof(char) * strlen("GE") + 1);
			strcpy(s, "GE");
			break;
		case 275 :
			s = check_malloc(sizeof(char) * strlen("AND") + 1);
			strcpy(s, "AND");
			break;
		case 276 :
			s = check_malloc(sizeof(char) * strlen("OR") + 1);
			strcpy(s, "OR");
			break;
		case 277 :
			s = check_malloc(sizeof(char) * strlen("MAX") + 1);
			strcpy(s, "MAX");
			break;
		case 278 :
			s = check_malloc(sizeof(char) * strlen("MIN") + 1);
			strcpy(s, "MIN");
			break;
		case 279 :
			s = check_malloc(sizeof(char) * strlen("AVG") + 1);
			strcpy(s, "AVG");
			break;
		case 280 :
			s = check_malloc(sizeof(char) * strlen("COUNT") + 1);
			strcpy(s, "COUNT");
			break;
		case 281 :
			s = check_malloc(sizeof(char) * strlen("SUM") + 1);
			strcpy(s, "SUM");
			break;
		case 282 :
			s = check_malloc(sizeof(char) * strlen("SEL") + 1);
			strcpy(s, "SEL");
			break;
		case 283 :
			s = check_malloc(sizeof(char) * strlen("WHERE") + 1);
			strcpy(s, "WHERE");
			break;
		case 284 :
			s = check_malloc(sizeof(char) * strlen("FROM") + 1);
			strcpy(s, "FROM");
			break;
		case 285 :
			s = check_malloc(sizeof(char) * strlen("EXPR_SEL") + 1);
			strcpy(s, "EXPR_SEL");
			break;
		case 286 :
			s = check_malloc(sizeof(char) * strlen("EXPR_FROM") + 1);
			strcpy(s, "EXPR_FROM");
			break;
		case 287 :
			s = check_malloc(sizeof(char) * strlen("EXPR_WHERE") + 1);
			strcpy(s, "EXPR_WHERE");
			break;
        case 288 :
            s = check_malloc(sizeof(char) * strlen("EXPR_LEFT") + 1);
			strcpy(s, "EXPR_LEFT");
			break;
        case 289 :
            s = check_malloc(sizeof(char) * strlen("EXPR_RIGHT") + 1);
			strcpy(s, "EXPR_RIGHT");
			break;
		case 290 :
			s = check_malloc(sizeof(char) * strlen("RENAME") + 1);
			strcpy(s, "RENAME");
			break;
		default :
			s = check_malloc(sizeof(char) * strlen("ERROR") + 1);
			strcpy(s, "ERROR");
			break;
	}
	return s;
}
