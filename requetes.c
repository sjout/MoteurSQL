#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "tables.h"
#include "semantic.h"
#include "parser.h"
#include "lexer.h"
#include "requetes.h"

struct Table         *array_from = 0;
int                   size_from  = 0;

static struct select *array_sel = 0;
static int            size_sel  = 0;

extern struct Table *array_tables;
extern int           size;

conditionJoin ConditionJoin()
{
    conditionJoin New = check_malloc(sizeof(struct conditionJoin_));

    New->size = 7, New->count_element = 0;
    New->T = check_malloc(sizeof(tree) * New->size);

    return New;
}
conditionSelect ConditionSelect()
{
    conditionSelect New = check_malloc(sizeof(struct conditionSelect_));

    New->size = 7, New->count_element = 0;
    New->T = check_malloc(sizeof(tree) * New->size);

    return New;
}
nodeTree NodeTree(int type, conditionJoin Cjoin, conditionSelect Csel, struct Table *T)
{
    nodeTree New = check_malloc(sizeof(struct nodeTree_));

    New->type = type;
    if (type == COND_JOIN)
        New->u.CJ = Cjoin;
    else if (type == TABLE_SEL)
        New->u.CS.C = Csel, New->u.CS.T = T;
    else if (type != PROD_CARTE)
    {
        fprintf(stderr, "Erreur : Type de condition inconnu.\n");
        exit(EXIT_FAILURE);
    }

    return New;
}
joinTree JoinTree(nodeTree N, joinTree left, joinTree right, joinTree father)
{
    joinTree T = check_malloc(sizeof(struct joinTree_));

    T->val = N;
    T->left = left, T->right = right, T->father = father;

    return T;
}

void add_conditionJoin(conditionJoin Cjoin, tree T)
{
    if (Cjoin->count_element == Cjoin->size)
    {
        Cjoin->size *= 2;
        check_realloc(Cjoin->T, sizeof(tree) * Cjoin->size);
    }
    Cjoin->T[Cjoin->count_element++] = T;
}
void add_conditionSelect(conditionSelect Csel, tree T)
{
    if (Csel->count_element == Csel->size)
    {
        Csel->size *= 2;
        check_realloc(Csel->T, sizeof(tree) * Csel->size);
    }
    Csel->T[Csel->count_element++] = T;
}

struct Table *getTable(const char *name)
{
    int i = 0;

    for (i = 0; i < size; i++)
        if (!strcmp(name, array_tables[i].name))
            return &(array_tables[i]);

    return 0;
}
struct Table *getTable_from(const char *name)
{
    int i = 0;

    for (i = 0; i < size_from; i++)
        if (!strcmp(name, array_from[i].name))
            return &(array_from[i]);

    return 0;
}
int getColumn(struct Table *T, char *str)
{
    int i = 0;
    for (i = 0; i < T->width; i++)
        if (!(strcmp(str, T->columns[i].name)))
            return i;
    return -1;
}
struct Table *findTable(const char *col)
{
    int i = 0, j = 0;

    for (i = 0; i < size_from; i++)
        for (j = 0; j < array_from[i].width; j++)
            if (!strcmp(col, array_from[i].columns[j].name))
                return &(array_from[i]);

    return 0;
}

void set_array_sel(tree select)
{
    int i;
    list tmp = select->L;
    tree param = 0;

    while (tmp != 0)
    {
        size_sel++;
        tmp = tmp->next;
    }
    tmp = select->L;
    array_sel = check_malloc(sizeof(struct select) * size_sel);

    for (i = 0; i < size_sel; i++)
    {
        param = 0;
        get_dot(tmp->T->L->T, &param);

        if (param != 0)
        {
            array_sel[i].T = getTable_from(param->L->T->T->u.str);
            array_sel[i].col = getColumn(array_sel[i].T, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(tmp->T->L->T, &param);
            array_sel[i].T = findTable(param->T->u.str);
            array_sel[i].col = getColumn(array_sel[i].T, param->T->u.str);
        }
        tmp = tmp->next;
    }
}
void set_array_from(tree from)
{
    struct Table *T = 0;
    list li = from->L;
    int i = 0;

    while (li != 0)
    {
        size_from++;
        li = li->next;
    }
    array_from = check_malloc(sizeof(struct Table) * size_from);

    li = from->L;

    while (li != 0 && i < size_from)
    {
        if (li->T->T->type == RENAME)
        {
            T = getTable(li->T->L->T->T->u.str);
            if (T == 0)
            {
                fprintf(stderr, "Erreur : Table -> %s inconnu.\n", li->T->L->T->T->u.str);
                exit(EXIT_FAILURE);
            }
            array_from[i] = *T;
            array_from[i++].name = li->T->L->next->T->T->u.str;
        }
        else
        {
            T = getTable(li->T->T->u.str);
            if (T == 0)
            {
                fprintf(stderr, "Erreur : Table -> %s inconnu.\n", li->T->T->u.str);
                exit(EXIT_FAILURE);
            }
            array_from[i++] = *T;
        }
        li = li->next;
    }
}
joinTree init_tree()
{
    assert(size_from > 0);
    int index = size_from - 1;
    joinTree root = 0, tmp = root;
    nodeTree new = 0;

    if (size_from == 1)
        return JoinTree(NodeTree(TABLE_SEL, 0, 0, &(array_from[index])), 0, 0, 0);
    root = JoinTree(NodeTree(PROD_CARTE, 0, 0, 0), 0, 0, 0), tmp = root;

    for (; index >= 0; index--)
    {
        new = NodeTree(TABLE_SEL, 0, 0, &(array_from[index]));
        if (index != 0)
            tmp->right = JoinTree(new, 0, 0, tmp);
        else
            tmp->left = JoinTree(new, 0, 0, tmp);
        if (index > 1)
        {
            tmp->left = JoinTree(NodeTree(PROD_CARTE, 0, 0, 0), 0, 0, tmp);
            tmp = tmp->left;
        }
    }

    return root;
}
void is_id_in(tree T, const char *str, tree *param, int *count_ID)
{
    assert(str != 0);
    if (T == 0)
        return;
    if (T->T->type == ID && T == T->father->L->T)
        (*count_ID)++;
    if (T->T->type == ID && !(strcmp(T->T->u.str, str)))
        *param = T;
    if (T->L != 0)
        is_id_in(T->L->T, str, param, count_ID);
    if (T->L != 0 && T->L->next != 0)
        is_id_in(T->L->next->T, str, param, count_ID);
}
void get_id(tree T, tree *param)
{
    if (T == 0)
        return;
    if (T->T->type == ID && ((T->father == 0) || (T->father != 0 && T->father->T->type != DOT)))
        *param = T;
    else if (T->father != 0 && T->father->T->type == DOT)
        *param = T->father->L->next->T;
    if (T->L != 0)
        get_id(T->L->T, param);
    if (T->L != 0 && T->L->next != 0)
        get_id(T->L->next->T, param);
}
void get_dot(tree T, tree *param)
{
    if (T == 0)
        return;
    if (T->T->type == DOT)
        *param = T;
    if (T->L != 0)
        get_dot(T->L->T, param);
    if (T->L != 0 && T->L->next != 0)
        get_dot(T->L->next->T, param);
}
void count_id(tree T, int *count_ID)
{
    if (T == 0)
        return;
    if (T->T->type == ID && T == T->father->L->T)
        (*count_ID)++;
    if (T->L != 0)
        count_id(T->L->T, count_ID);
    if (T->L != 0 && T->L->next != 0)
        count_id(T->L->next->T, count_ID);
}
void set_tree(tree conds, joinTree T)
{
    assert(T != 0);
    int count_ID = 0, ind = 0, k, get = 0;
    conditionJoin Cjoin = 0, Resjoin = 0;
    conditionSelect Csel = 0, Ressel = 0;
    tree TMP = conds;
    joinTree JOIN = T;
    bool find = FALSE;

    if (conds == 0)
        return;

    Cjoin = ConditionJoin();
    Csel = ConditionSelect();

    while (TMP->L != 0 && is_operator_join(TMP->T->type))
    {
        count_ID = 0;
        count_id(TMP->L->next->T, &count_ID);
        if (count_ID > 1)
        {
            if (Cjoin == 0)
                Cjoin = ConditionJoin();
            add_conditionJoin(Cjoin, TMP->L->next->T);
        }
        else
        {
            if (Csel == 0)
                Csel = ConditionSelect();
            add_conditionSelect(Csel, TMP->L->next->T);
        }
        TMP = TMP->L->T;
    }
    count_ID = 0;
    count_id(TMP, &count_ID);
    if (count_ID > 1)
    {
        if (Cjoin == 0)
            Cjoin = ConditionJoin();
        add_conditionJoin(Cjoin, TMP);
    }
    else
    {
        if (Csel == 0)
            Csel = ConditionSelect();
        add_conditionSelect(Csel, TMP);
    }
    while (JOIN != 0)
    {
        Resjoin = 0; Ressel = 0;
        if (JOIN->right != 0)
        {
            for (ind = 0; ind < Cjoin->count_element; ind++)
            {
                TMP = 0, count_ID = 0, get = -1, find = FALSE;
                is_id_in(Cjoin->T[ind], JOIN->right->val->u.CS.T->name, &TMP, &count_ID);
                if (TMP == 0)
                {
                    get_id(Cjoin->T[ind], &TMP);
                    if (TMP->father == 0 || (TMP->father != 0 && TMP->father->T->type != DOT) || !strcmp(JOIN->right->val->u.CS.T->name, TMP->father->L->T->T->u.str))
                        get = getColumn(JOIN->right->val->u.CS.T, TMP->T->u.str);

                    if (get != -1)
                        find = TRUE;
                    TMP = 0;
                }
                if (TMP != 0 || find)
                {
                    if (Resjoin == 0)
                        Resjoin = ConditionJoin();
                    add_conditionJoin(Resjoin, Cjoin->T[ind]);

                    for (k = ind; k < Cjoin->count_element - 1; k++)
                        Cjoin->T[k] = Cjoin->T[k + 1];
                    Cjoin->count_element -= 1;
                    ind -= 1;
                }
            }
            for (ind = 0; ind < Csel->count_element; ind++)
            {
                TMP = 0, count_ID = 0, get = -1, find = FALSE;
                is_id_in(Csel->T[ind], JOIN->right->val->u.CS.T->name, &TMP, &count_ID);

                if (TMP == 0)
                {
                    get_id(Csel->T[ind]->L->T, &TMP);
                    if (TMP->father == 0 || (TMP->father != 0 && TMP->father->T->type != DOT) || !strcmp(JOIN->right->val->u.CS.T->name, TMP->father->L->T->T->u.str))
                        get = getColumn(JOIN->right->val->u.CS.T, TMP->T->u.str);
                    if (get != -1)
                        find = TRUE;
                    TMP = 0;
                }
                if (TMP != 0 || find)
                {
                    if (Ressel == 0)
                        Ressel = ConditionSelect();
                    add_conditionSelect(Ressel, Csel->T[ind]);
                    for (k = ind; k < Cjoin->count_element - 1; k++)
                        Cjoin->T[k] = Cjoin->T[k + 1];
                    Cjoin->count_element -= 1;
                }
            }
            if (Ressel != 0)
                JOIN->right->val->u.CS.C = Ressel;
            if (Resjoin != 0)
            {
                free(JOIN->val);
                JOIN->val = NodeTree(COND_JOIN, Resjoin, 0, 0);
            }
        }
        Ressel = 0;
        if (JOIN->left == 0)
        {
            for (ind = 0; ind < Csel->count_element; ind++)
            {
                TMP = 0, count_ID = 0, get = -1, find = FALSE;
                is_id_in(Csel->T[ind], JOIN->val->u.CS.T->name, &TMP, &count_ID);
                if (TMP == 0)
                {
                    get_id(Csel->T[ind], &TMP);
                    if (TMP->father == 0 || (TMP->father != 0 && TMP->father->T->type != DOT) || !strcmp(JOIN->val->u.CS.T->name, TMP->father->L->T->T->u.str))
                        get = getColumn(JOIN->val->u.CS.T, TMP->T->u.str);
                    if (get != -1)
                        find = TRUE;
                    TMP = 0;
                }
                if (TMP != 0 || find)
                {
                    if (Ressel == 0)
                        Ressel = ConditionSelect();
                    add_conditionSelect(Ressel, Csel->T[ind]);
                    for (k = ind; k < Cjoin->count_element - 1; k++)
                        Cjoin->T[k] = Cjoin->T[k + 1];
                    Cjoin->count_element -= 1;
                }
            }
            if (Ressel != 0)
                JOIN->val->u.CS.C = Ressel;
        }
        JOIN = JOIN->left;
    }

    free(Cjoin->T), free(Cjoin);
    free(Csel->T), free(Csel);
}
int calculus(tree T, int val)
{
    int left = 0, right = 0;

    if (T == 0)
        return 0;
    if (T->T->type == ID)
        return val;
    if (T->L == 0)
        return T->T->u.val;
    if (T->T->type == DOT)
        return val;
    left = calculus(T->L->T, val);
    right= calculus(T->L->next->T, val);
    if (T->T->type == PLUS)
        return left + right;
    else if (T->T->type == MINUS)
        return left - right;
    else if (T->T->type == TIMES)
        return left * right;
    else if (T->T->type == DIVIDE)
        return left / right;

    return 0;
}

int getIndex(struct pos *indices, const int max_len, struct Table *T)
{
    int i;
    for (i = 0; i < max_len; i++)
    {
        if (T == indices[i].T)
            return indices[i].index;

    }
    return -1;
}
bool check_select(conditionSelect Csel, int index, struct Table *T)
{
    int i = 0, col = 0;
    int left_int = 0, right_int= 0;
    string left_str = 0, right_str = 0;
    tree param = 0;
    bool check = TRUE, left = FALSE, right = FALSE;

    for (i = 0; i < Csel->count_element && check; i++)
    {
        param = 0;
        left = FALSE, right = FALSE;

        get_id(Csel->T[i]->L->T, &param);
        if (param != 0)
            left = TRUE;
        if (!left)
            get_id(Csel->T[i]->L->next->T, &param), right = TRUE;
        col = getColumn(T, param->T->u.str);
        if (col == -1)
        {
            fprintf(stderr, "Erreur : Colonne \"%s\" inconnu.\n", param->T->u.str);
            exit(EXIT_FAILURE);
        }
        if (T->columns[col].is_string)
        {
            if (left)
            {
                left_str = String(T->content[index * T->width + col].str);
                right_str = String(Csel->T[i]->L->next->T->T->u.str);
            }
            else
            {
                left_str = String(Csel->T[i]->L->T->T->u.str);
                right_str = String(T->content[index * T->width + col].str);
            }
            switch(Csel->T[i]->T->type)
            {
            case EQ :
                if (strcmp(left_str, right_str))
                    check = FALSE;
                break;
            case NEQ :
                if (!strcmp(left_str, right_str))
                    check = FALSE;
                break;
            }
            free(left_str), free(right_str);
        }
        else
        {
            if (left)
            {
                left_int = calculus(Csel->T[i]->L->T, T->content[index * T->width + col].value);
                right_int = calculus(Csel->T[i]->L->next->T, 0);
            }
            else
            {
                left_int = calculus(Csel->T[i]->L->T, 0);
                right_int = calculus(Csel->T[i]->L->next->T, T->content[index * T->width + col].value);
            }
            switch(Csel->T[i]->T->type)
            {
            case EQ :
                if (left_int != right_int)
                    check = FALSE;
                break;
            case NEQ :
                if (left_int == right_int)
                    check = FALSE;
                break;
            case LT :
                if (left_int >= right_int)
                    check = FALSE;
                break;
            case GT :
                if (left_int <= right_int)
                    check = FALSE;
                break;
            case LE :
                if (left_int > right_int)
                    check = FALSE;
                break;
            case GE :
                if (left_int < right_int)
                    check = FALSE;
                break;
            }
        }
    }

    return check;
}
bool check_join(conditionJoin Cjoin, int index, struct Table *T, struct pos *indices, int curr_len)
{
    bool check = TRUE;
    struct Table *left_table = 0, *right_table = 0;
    string left_str = 0, right_str = 0;
    int i = 0, left_int = 0, right_int = 0, col_left = 0, col_right = 0;
    int new_ind = 0;
    tree param = 0;

    for (i = 0; i < Cjoin->count_element && check; i++)
    {
        param = 0;
        get_dot(Cjoin->T[i]->L->T, &param);
        if (param != 0)
        {
            left_table = getTable_from(param->L->T->T->u.str);
            col_left = getColumn(left_table, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(Cjoin->T[i]->L->T, &param);
            left_table = findTable(param->T->u.str);
            col_left = getColumn(left_table, param->T->u.str);
        }
        param = 0;

        get_dot(Cjoin->T[i]->L->next->T, &param);
        if (param != 0)
        {
            right_table = getTable_from(param->L->T->T->u.str);
            col_right = getColumn(right_table, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(Cjoin->T[i]->L->next->T, &param);
            right_table = findTable(param->T->u.str);
            col_right = getColumn(right_table, param->T->u.str);
        }
        if (left_table == T)
        {
            new_ind = getIndex(indices, curr_len, right_table);
            if (left_table->columns[col_left].is_string)
            {
                left_str = String(left_table->content[index * left_table->width + col_left].str);
                right_str = String(right_table->content[new_ind * right_table->width + col_right].str);

                switch(Cjoin->T[i]->T->type)
                {
                case EQ :
                    if (strcmp(left_str, right_str))
                        check = FALSE;
                    break;
                case NEQ :
                    if (!strcmp(left_str, right_str))
                        check = FALSE;
                    break;
                }
                free(left_str), free(right_str);
            }
            else
            {
                left_int = calculus(Cjoin->T[i]->L->T, left_table->content[index * left_table->width + col_left].value);
                right_int = calculus(Cjoin->T[i]->L->next->T, right_table->content[new_ind * right_table->width + col_right].value);

                switch(Cjoin->T[i]->T->type)
                {
                case EQ :
                    if (left_int != right_int)
                        check = FALSE;
                    break;
                case NEQ :
                    if (left_int == right_int)
                        check = FALSE;
                    break;
                case LT :
                    if (left_int >= right_int)
                        check = FALSE;
                    break;
                case GT :
                    if (left_int <= right_int)
                        check = FALSE;
                    break;
                case LE :
                    if (left_int > right_int)
                        check = FALSE;
                    break;
                case GE :
                    if (left_int < right_int)
                        check = FALSE;
                    break;
                }
            }
        }
        else
        {
            new_ind = getIndex(indices, curr_len, left_table);
            if (left_table->columns[col_left].is_string)
            {
                left_str = String(left_table->content[new_ind * left_table->width + col_left].str);
                right_str = String(right_table->content[index * right_table->width + col_right].str);

                switch(Cjoin->T[i]->T->type)
                {
                case EQ :
                    if (strcmp(left_str, right_str))
                        check = FALSE;
                    break;
                case NEQ :
                    if (!strcmp(left_str, right_str))
                        check = FALSE;
                    break;
                }
                free(left_str), free(right_str);
            }
            else
            {
                left_int = calculus(Cjoin->T[i]->L->T, left_table->content[new_ind * left_table->width + col_left].value);
                right_int = calculus(Cjoin->T[i]->L->next->T, right_table->content[index * right_table->width + col_right].value);
                switch(Cjoin->T[i]->T->type)
                {
                case EQ :
                    if (left_int != right_int)
                        check = FALSE;
                    break;
                case NEQ :
                    if (left_int == right_int)
                        check = FALSE;
                    break;
                case LT :
                    if (left_int >= right_int)
                        check = FALSE;
                    break;
                case GT :
                    if (left_int <= right_int)
                        check = FALSE;
                    break;
                case LE :
                    if (left_int > right_int)
                        check = FALSE;
                    break;
                case GE :
                    if (left_int < right_int)
                        check = FALSE;
                    break;
                }
            }
        }
    }

    return check;
}
void nestedLoop(joinTree T, struct pos *indices, const int max_len, int curr_len)
{
    int i = 0, j = 0;

    if (T == 0)
    {
        for (i = 0; i < size_sel; i++)
        {
            for (j = 0; j < max_len; j++)
            {
                if (array_sel[i].T == indices[j].T)
                {
                    if (indices[j].T->columns[array_sel[i].col].is_string)
                        printf("%10s ", indices[j].T->content[indices[j].T->width * indices[j].index + array_sel[i].col].str);
                    else
                    printf("%10d ", indices[j].T->content[indices[j].T->width * indices[j].index + array_sel[i].col].value);
                }
            }
        }
        printf("\n");

        return;
    }

    if (T->val->type == TABLE_SEL)
    {
        indices[curr_len].T = T->val->u.CS.T;
        indices[curr_len].index = 0;

        for (i = 0; i < T->val->u.CS.T->length; i++)
        {
            indices[curr_len].index = i;
            if (T->val->u.CS.C == 0)
                nestedLoop(T->father, indices, max_len, curr_len + 1);
            else if (check_select(T->val->u.CS.C, i, T->val->u.CS.T))
                nestedLoop(T->father, indices, max_len, curr_len + 1);
        }
    }
    else if (T->val->type == PROD_CARTE)
    {
        indices[curr_len].T = T->right->val->u.CS.T;
        indices[curr_len].index = 0;

        for (i = 0; i < T->right->val->u.CS.T->length; i++)
        {
            indices[curr_len].index = i;
            if (T->right->val->u.CS.C == 0)
                nestedLoop(T->father, indices, max_len, curr_len + 1);
            else if (check_select(T->right->val->u.CS.C, i, T->right->val->u.CS.T))
                nestedLoop(T->father, indices, max_len, curr_len + 1);
        }
    }
    else
    {
        indices[curr_len].T = T->right->val->u.CS.T;
        indices[curr_len].index = 0;

        for (i = 0; i < T->right->val->u.CS.T->length; i++)
        {
            indices[curr_len].index = i;
            if (T->right->val->u.CS.C == 0)
            {
                if (check_join(T->val->u.CJ, i, T->right->val->u.CS.T, indices, curr_len))
                    nestedLoop(T->father, indices, max_len, curr_len + 1);
            }
            else if (check_select(T->right->val->u.CS.C, i, T->right->val->u.CS.T))
            {
                if (check_join(T->val->u.CJ, i, T->right->val->u.CS.T, indices, curr_len))
                    nestedLoop(T->father, indices, max_len, curr_len + 1);
            }
        }
    }
}

void engine(char *csv, char *sql)
{
    FILE *code = 0;
    tree query = 0;
    joinTree join = 0, parcour = 0;
    list tmp = 0;
    struct pos *indices = 0;
    int i;

    /*  Stockage des tables du fichier .csv */
    set_tables(csv);
    /*  Création d'un arbre de syntaxe abstraite */
    code = check_open(sql);
    query = AST(code);

    /*  Stockage du SELECT et du FROM */
    semantic_expr_from(query->L->next->T);      /*  Analyse sémantique du FROM */
    set_array_from(query->L->next->T->L->T);
    semantic_expr_sel(query->L->T);             /*  Analyse sémantique du SELECT */
    set_array_sel(query->L->T);

    /*  Création de l'arbre de jointure */
    join = init_tree();
    tmp = query->L;
    while (tmp != 0 && tmp->T->T->type != WHERE)
        tmp = tmp->next;
    if (tmp != 0)
    {
        semantic_expr_where(tmp->T->L->T);
        set_tree(tmp->T->L->T->L->T, join);
    }

    /*  Parcours pour démarrer de la dernière feuille la plus à gauche */
    parcour = join;
    indices = check_malloc(sizeof(struct pos) * size_from);
    parcour = join;
    while (parcour->left != 0)
        parcour = parcour->left;

    /*  Affichage des nom de colonnes du SELECT */
    for (i = 0; i < size_sel; i++)
        printf("%10s ", array_sel[i].T->columns[array_sel[i].col].name);
    printf("\n");

    /*  Application de l'algorithme du parcours de l'arbre de JOINTURE */
    nestedLoop(parcour, indices, size_from, 0);

    /*  Libération de la mémoire */
    free_joinTree(join);
    free(indices);
    free_ast(query);
    free(array_sel), free(array_from);
    free_tables();
    fclose(code);
}

void free_joinTree(joinTree T)
{
    assert(T != 0);
    joinTree tmp = T;
    if (T->left == 0)
    {
        if (T->val->u.CS.C != 0)
        {
            free(T->val->u.CS.C->T);
            free(T->val->u.CS.C);
        }
        free(T->val);
        free(T);
    }
    else
    {
        tmp = T->left;
        if (T->right->val->u.CS.C != 0)
        {
            free(T->right->val->u.CS.C->T);
            free(T->right->val->u.CS.C);
        }
        free(T->right->val);
        free(T->right);

        if (T->val->type == COND_JOIN)
        {
            if (T->val->u.CJ != 0)
            {
                free(T->val->u.CJ->T);
                free(T->val->u.CJ);
            }
            free(T->val);
            free(T);
        }
        else
            free(T->val), free(T);

        free_joinTree(tmp);
    }
}
