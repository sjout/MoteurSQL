#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "lexer.h"
#include "parser.h"
#include "stack.h"

static int delimiter[] =
{
    COMMA,
    SEMICOLON
};

static int op_bin[] =
{
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
};

static int operator_comp[] =
{
    EQ,
    NEQ,
    LT,
    LE,
    GT,
    GE
};

static int operator_join[] =
{
    AND,
    OR
};

static int keyword[] =
{
    SEL,
    FROM,
    WHERE
};

static int agregate[] =
{
    MAX,
    MIN,
    AVG,
    COUNT,
    SUM
};

list List(tree T, list L)
{
    list New = check_malloc(sizeof(struct list_));

    New->T = T, New->next = L;

    return New;
}
tree Tree(token T, tree F, list L)
{
    tree New = check_malloc(sizeof(struct tree_));

    New->T = T, New->father = F, New->L = L;

    return New;
}

void add_tree(tree T, tree N)
{
    assert(T != 0);
    list New = List(N, 0);
    list ptr = T->L;

    N->father = T;

    if (ptr == 0)
        T->L = New;
    else
    {
        while (ptr->next != 0)
            ptr = ptr->next;
        ptr->next = New;
    }
}
void add_list(list L, list N)
{
    assert(L != 0 && N != 0);
    list T = L;

    while (T->next != 0)
        T = T->next;
    T->next = N;
}
list get_last(list L)
{
    assert(L != 0);
    list tmp = L;

    while (tmp->next != 0)
        tmp = tmp->next;

    return tmp;

}
list get_last_and_replace(tree T)
{
    assert(T != 0 && T->L != 0);
    list tmp = T->L, tmp2 = 0;

    while (tmp->next != 0)
        tmp2 = tmp, tmp = tmp->next;
    if (tmp2 == 0)
        T->L = 0;
    else
        tmp2->next = 0;

    return tmp;
}

bool is_delimiter(int x)
{
    bool in = FALSE;
    int i = 0;

    for (i = 0; i < sizeof(delimiter) / sizeof(*delimiter) && !in; i++)
        if (delimiter[i] == x)
            in = TRUE;

    return in;
}
bool is_operator_comp(int x)
{
    bool in = FALSE;
    int i = 0;

    for (i = 0; i < sizeof(operator_comp) / sizeof(*operator_comp) && !in; i++)
        if (operator_comp[i] == x)
            in = TRUE;

    return in;
}
bool is_operator_join(int x)
{
    bool in = FALSE;
    int i = 0;

    for (i = 0; i < sizeof(operator_join) / sizeof(*operator_join) && !in; i++)
        if (operator_join[i] == x)
            in = TRUE;

    return in;
}
bool is_operator_arith(int x)
{
    bool in = FALSE;
    int i = 0;

    for (i = 0; i < sizeof(op_bin) / sizeof(*op_bin) && !in; i++)
        if (op_bin[i] == x)
            in = TRUE;

    return in;
}
bool is_keyword(int x)
{
    bool in = FALSE;
    int i = 0;

    for (i = 0; i < (sizeof(keyword) / sizeof(*keyword)) && !in; i++)
        if (keyword[i] == x)
            in = TRUE;

    return in;
}
bool is_agregate(int x)
{
    bool in = FALSE;
    int i = 0;

    for (i = 0; i < (sizeof(agregate) / sizeof(*agregate)) && !in; i++)
        if (agregate[i] == x)
            in = TRUE;

    return in;
}

void expr_select(queue Q, tree Expression, int *count_ID, token *R)
{
    tree New = 0, Sep = 0, New2 = 0;
    token T = dequeue(Q);
    *R = T;

    if (T != 0 && !is_keyword(T->type) && !is_delimiter(T->type))
    {
        New = Tree(T, 0, 0);
        if (T->type == ID)
        {
            if ((*count_ID) == 1)
                lexical_error_select(S_COL_ERR);
            T = dequeue(Q);
            *R = T;
            if (T != 0 && (T->type == COMMA || is_keyword(T->type)))
            {
                add_tree(Expression, New);
                return;
            }
            if (T != 0 && T->type != DOT)
                lexical_error_select(S_ID_ERR);
            Sep = Tree(T, Expression, 0);
            if ((T = dequeue(Q)) != 0 && T->type != ID)
                lexical_error_select(S_COL_ERR);
            *R = T;
            New2 = Tree(T, Sep, 0);
            New->father = Sep;
            add_tree(Sep, New), add_tree(Sep, New2);
            add_tree(Expression, Sep);
            (*count_ID)++;
            if ((T = dequeue(Q)) != 0 && T->type != COMMA && !is_keyword(T->type))
                lexical_error_select(S_ID_ERR);
            *R = T;

            return;
        }
        else
            lexical_error_select(S_NOT_GRAM_ERR);
    }
    else
        return;
}
void expr_from(queue Q, tree Expression, int *count_ID, token *R)
{
    tree New = 0, New2 = 0, Table = 0;
    token T = dequeue(Q);
    *R = T;
    if (T != 0 && !is_keyword(T->type) && !is_delimiter(T->type))
    {
        New = Tree(T, Table, 0);
        if (T->type == ID)
        {
            if ((*count_ID) == 1)
                lexical_error_from(F_ID_ERR);
            (*count_ID)++;
            if ((T = dequeue(Q)) != 0 && T->type != ID && T->type != COMMA && T->type != SEMICOLON && !is_keyword(T->type))
                lexical_error_from(F_RENAME_ERR);
            if (T == 0)
                lexical_error_from(F_NOT_FULL);
            else if (T->type == COMMA || T->type == SEMICOLON || is_keyword(T->type))
            {
                add_tree(Expression, New);
                *R = T;
                return;
            }
            Table = Tree(Token(RENAME, 0, 0), Expression, 0);
            add_tree(Table, New), New->father = Table;
            New2 = Tree(T, Table, 0);
            add_tree(Table, New2);
            add_tree(Expression, Table);
            if ((T = dequeue(Q)) != 0 && T->type != COMMA && T->type != SEMICOLON && !is_keyword(T->type))
                lexical_error_from(F_RENAME_ERR);
            if (T == 0)
                lexical_error_from(F_NOT_FULL);
            *R = T;
        }
        else
            lexical_error_from(F_NOT_GRAM_ERR);
    }
    else
        return;
}
void expr_where(queue Q, tree Expression, int *count_ID, token *R, stack S)
{
    int x;
    tree New = 0, Sep = 0, New2 = 0;
    list Li = 0;
    token T = dequeue(Q);
    *R = T;
    if (T != 0 && !is_keyword(T->type) && !is_delimiter(T->type) && !is_operator_join(T->type) && !is_operator_comp(T->type))
    {
        New = Tree(T, 0, 0);
        if (T->type == LPAREN)
        {
            free(New->T), free(New);
            push(S);
            New2 = Tree(Token(EXPR_WHERE, 0, 0), 0, 0);
            expr_where(Q, New2, count_ID, R, S);
            New2->father = Expression;
            if (New2->L != 0)
            {
                add_tree(Expression, New2->L->T);
                free(New2->T), free(New2->L), free(New2);
            }
            else
                lexical_error_where(W_EMPTY_PAREN_ERR);
            if (Expression->T->type == TIMES || Expression->T->type == DIVIDE)
                expr_where(Q, Expression->father, count_ID, R, S);
            else
                expr_where(Q, Expression, count_ID, R, S);

        }
        else if (T->type == RPAREN)
        {
            free(New->T), free(New);
            x = pop(S);
            if (Expression->father == 0 && Expression->L == 0)
                lexical_error_where(W_EMPTY_PAREN_ERR);
            else if (x == 0)
                lexical_error_where(W_PAREN_ERR);
        }
        else if (T->type == ID)
        {
            if ((*count_ID) == 1)
                lexical_error_where(W_COL_ERR);
            T = dequeue(Q);
            if (T != 0 && (T->type == SEMICOLON || is_keyword(T->type) || is_operator_comp(T->type) || is_operator_arith(T->type) || is_operator_join(T->type)))
            {
                *R = T;
                add_tree(Expression, New);
                if (T->type == SEMICOLON || is_operator_join(T->type) || is_operator_comp(T->type) || is_keyword(T->type))
                    return;
                if (Expression->T->type == TIMES || Expression->T->type == DIVIDE)
                    expr_where(Q, Expression->father, count_ID, R, S);
                else
                    expr_where(Q, Expression, count_ID, R, S);
                return;
            }
            if (T != 0 && T->type != DOT)
                lexical_error_where(W_ID_ERR);
            Sep = Tree(T, Expression, 0);
            if ((T = dequeue(Q)) != 0 && T->type != ID)
                lexical_error_where(W_COL_ERR);
            *R = T;
            New2 = Tree(T, Sep, 0);
            New->father = Sep;
            add_tree(Sep, New), add_tree(Sep, New2);
            add_tree(Expression, Sep);
            (*count_ID)++;
            if (Expression->T->type == TIMES || Expression->T->type == DIVIDE)
                expr_where(Q, Expression->father, count_ID, R, S);
            else
                expr_where(Q, Expression, count_ID, R, S);
        }
        else if (is_operator_arith(T->type))
        {
            if (Expression->L == 0)
                lexical_error_where(W_OPERANDE_ERR);
            Li = get_last_and_replace(Expression);
            add_tree(New, Li->T);
            Li->T->father = New;
            New->father = Expression;
            add_tree(Expression, New);
            free(Li);
            expr_where(Q, New, count_ID, R, S);
        }
        else if (T->type == INT)
        {
            if (Expression->L != 0 && (Expression->T->type == EXPR_WHERE || !is_operator_arith(Expression->T->type) || Expression->L->next != 0))
                lexical_error_where(W_OP_ERR);
            add_tree(Expression, New);
            if (Expression->T->type == DIVIDE || Expression->T->type == TIMES)
                expr_where(Q, Expression->father, count_ID, R, S);
            else
                expr_where(Q, Expression, count_ID, R, S);
        }
        else if (T->type == STRING)
        {
            if (Expression->L != 0 && !is_operator_comp(Expression->T->type) && Expression->T->type != EXPR_WHERE)
                lexical_error_where(W_STR_FAIL);
            add_tree(Expression, New);
            T = dequeue(Q);
            *R = T;
            if (!is_operator_comp(T->type) && !is_keyword(T->type) && T->type != COMMA && T->type != SEMICOLON && !is_operator_join(T->type))
                lexical_error_where(W_STR_FAIL);
        }
        else
            lexical_error_where(W_NOT_GRAM_ERR);
    } 
    else
    {
        x = pop(S);
        if (x != 0)
            lexical_error_where(W_PAREN_ERR);
        return;
    }
}
void expr_where_full(queue Q, tree Expression, token *R)
{
    tree left_op = Tree(Token(EXPR_WHERE, 0, 0), 0, 0);
    tree right_op = Tree(Token(EXPR_WHERE, 0, 0), 0, 0);
    tree operator = 0, new = 0;
    list li = 0;

    stack S = Stack();
    int count_ID = 0;

    expr_where(Q, left_op, &count_ID, R, S);
    if (left_op->L == 0)
        lexical_error_where(W_NO_LOPERANDE);
    if (*R != 0 && is_operator_comp((*R)->type))
    {
        operator = Tree(*R, 0, 0);
        left_op->L->T->father = operator;
        add_tree(operator, left_op->L->T);
        free(left_op->T);
        free(left_op->L);
        free(left_op);
    }
    else
        lexical_error_where(W_NO_OP);

    count_ID = 0;
    expr_where(Q, right_op, &count_ID, R, S);
    if (right_op->L == 0)
        lexical_error_where(W_NO_ROPERANDE);
    if (*R != 0 && (is_operator_join((*R)->type) || (*R)->type == SEMICOLON))
    {
        right_op->L->T->father = operator;
        add_tree(operator, right_op->L->T);
        free(right_op->T);
        free(right_op->L);
        free(right_op);
    }
    else
        lexical_error_where(W_NO_AND_SEMICOL);
    if (Expression->L != 0 && is_operator_join(Expression->L->T->T->type))
        add_tree(Expression->L->T, operator);

    if ((*R)->type == AND)
    {
        if (Expression->L == 0)
        {
            new = Tree(*R, Expression, 0);
            add_tree(new, operator);
            operator->father = new;
            add_tree(Expression, new);
        }
        else
        {
            li = get_last_and_replace(Expression);
            new = Tree(*R, Expression, li);
            add_tree(Expression, new);
        }
        expr_where_full(Q, Expression, R);
    }
    else if ((*R)->type == SEMICOLON)
    {
        if (Expression->L == 0 || (Expression->L != 0 && !is_operator_join(Expression->L->T->T->type)))
            add_tree(Expression, operator);
    }

    free_stack(S);
}

void query_tree(queue Q, tree T)
{
    assert(Q != 0 && T != 0);
    token R = dequeue(Q);
    tree tmp = 0, sel = 0, from = 0, where = 0;
    int count_ID = 0, set = 0;


    if (R->type != SEL)
        lexical_error_query(Q_NO_SEL);
    while (R != 0 && R->type != SEMICOLON)
    {
        count_ID = 0;
        if (R->type == SEL)
        {
            set = 1;
            sel = Tree(R, T, 0);
            add_tree(T, sel);
            tmp = Tree(Token(EXPR_SEL, 0, 0), sel, 0);
            add_tree(sel, tmp);
            expr_select(Q, tmp, &count_ID, &R);
            if (tmp->L == 0)
                lexical_error_query(Q_NO_ARGS_SEL);
        }
        else if (R->type == FROM)
        {
            set = 2;
            from = Tree(R, T, 0);
            add_tree(T, from);
            tmp = Tree(Token(EXPR_FROM, 0, 0), from, 0);
            add_tree(from, tmp);
            expr_from(Q, tmp, &count_ID, &R);
            if (tmp->L == 0)
                lexical_error_query(Q_NO_ARGS_FROM);
        }
        else if (R->type == WHERE)
        {
            set = 3;
            where = Tree(R, T, 0);
            add_tree(T, where);
            tmp = Tree(Token(EXPR_WHERE, 0, 0), where, 0);
            add_tree(where, tmp);
            expr_where_full(Q, tmp, &R);
            if (tmp->L == 0)
                lexical_error_query(Q_NO_ARGS_WHERE);
        }
        else
        {
            if (set == 1)
            {
                while (R != 0 && !is_keyword(R->type))
                {
                    free(R);
                    count_ID = 0;
                    tmp = Tree(Token(EXPR_SEL, 0, 0), sel, 0);
                    add_tree(sel, tmp);
                    expr_select(Q, tmp, &count_ID, &R);
                    if (tmp->L == 0)
                        lexical_error_select(S_NOT_GRAM_ERR);
                }
            }
            else if(set == 2)
            {
                while (R != 0 && !is_keyword(R->type) && R->type != SEMICOLON)
                {
                    free(R);
                    count_ID = 0;
                    expr_from(Q, tmp, &count_ID, &R);
                    if (tmp->L == 0)
                        lexical_error_from(F_NOT_GRAM_ERR);
                }
            }
            else if (set == 3)
                lexical_error_query(Q_NO_AFTER_WHERE);
        }
    }
    if (R == 0)
        lexical_error_query(Q_NO_SEMICOL);
    else
    {
        if (R->type == ID || R->type == STRING)
            free(R->u.str);
        free(R);
    }

    check_query(T);
}

void check_query(tree T)
{
    assert(T != 0);
    if (T->L == 0)
        lexical_error_query(Q_EMPTY);
    if ( T->L->T->T->type != SEL)
        lexical_error_query(Q_NO_SEL);
    if (T->L->next == 0)
        lexical_error_query(Q_NO_FROM);
    if (T->L->next->T->T->type != FROM)
        lexical_error_query(Q_NO_FROM);
    if (T->L->next->next == 0)
        return;
    if (T->L->next->next->T->T->type != WHERE)
        lexical_error_query(Q_NO_WHERE);
}

void print_tree(tree T, int i)
{
	list tmp = T->L;
    i++;
    if (T->L == 0) 
        return;
    while (tmp != 0)
    {
        printf("%d  ", tmp->T->T->type);
        if (tmp->T->T->type == INT)
            printf("Node (%d): %s - %d \n", i, id_to_str(tmp->T->T->type), tmp->T->T->u.val);
        else
            printf("Node (%d): %s\n", i, id_to_str(tmp->T->T->type));
        print_tree(tmp->T, i);
        tmp = tmp->next;
    }
}

tree AST(FILE *ptr)
{
	tree T = Tree(Token(QUERY, 0, 0), 0, 0);
    queue Q = lexer(ptr);
	query_tree(Q, T);

    free_lexer(Q);

	return T;
}

void free_ast(tree T)
{
    list tmp = T->L, backup = T->L;
    tree save = 0;

    if (T->L != 0) 
    {
        while (tmp != 0)
        {
            save = tmp->T;
            backup = tmp->next;
            free(tmp);
            T->L = backup;
            tmp = backup;
            free_ast(save);
        }
    }

    if (T->L == 0)
    {
        if (T->T->type == ID || T->T->type == STRING)
            free(T->T->u.str);
        free(T->T);
        free(T);
    }

    if (T == 0)
        return;
}
