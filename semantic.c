#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "util.h"
#include "tables.h"
#include "lexer.h"
#include "parser.h"
#include "requetes.h"
#include "semantic.h"

extern struct Table *array_from;
extern int           size_from;

extern struct Table *array_tables;
extern int           size;

void semantic_expr_sel(tree T)
{
    assert(T != 0);
    int count = 0, i = 0, j = 0;
    list TMP = T->L;
    tree param = 0;
    struct Table *table = 0;

    while (TMP != 0)
    {
        param = 0;
        count = 0;

        get_dot(TMP->T, &param);
        if (param != 0)
        {
            if ((table = getTable_from(param->L->T->T->u.str)) == 0)
                semantic_error(NOT_FOUND_TABLE, param->L->T->T->u.str);
            if (getColumn(table, param->L->next->T->T->u.str) == -1)
                semantic_error(NOT_FOUND_COL, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(TMP->T, &param);
            if (findTable(param->T->u.str) == 0)
                semantic_error(NOT_FOUND_COL, param->T->u.str);
            for (i = 0; i < size_from; i++)
            {
                for (j = 0; j < array_from[i].width; j++)
                {
                    if (!strcmp(array_from[i].columns[j].name, param->T->u.str))
                        count++;
                }
            }
            if (count > 1)
                semantic_error(IS_STRANGE, param->T->u.str);
        }
    TMP = TMP->next;
    }
}
void semantic_expr_from(tree T)
{
    assert(T != 0);
    list TMP = T->L;

    while (TMP != 0)
    {
        if (TMP->T->L->T->T->type == RENAME)
        {
            if (getTable(TMP->T->L->T->L->T->T->u.str) == 0)
                semantic_error(NOT_FOUND_TABLE, TMP->T->L->T->L->T->T->u.str);
        }
        else
            if (getTable(TMP->T->L->T->T->u.str) == 0)
                semantic_error(NOT_FOUND_TABLE, TMP->T->L->T->T->u.str);

        TMP = TMP->next;
    }
}
void semantic_expr_where(tree T)
{
    assert(T != 0);
    int count = 0, i = 0, j = 0;
    tree TMP = T->L->T, focus = 0, param =0;
    struct Table *table = 0;

    while (TMP != 0 && is_operator_join(TMP->T->type))
    {
        param = 0;
        count = 0;
        focus = TMP->L->next->T;
        get_dot(focus->L->T, &param);
        if (param != 0)
        {
            if ((table = getTable_from(param->L->T->T->u.str)) == 0)
                semantic_error(NOT_FOUND_TABLE, param->L->T->T->u.str);
            if (getColumn(table, param->L->next->T->T->u.str) == -1)
                semantic_error(NOT_FOUND_COL, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(focus->L->T, &param);
            if (param != 0 && findTable(param->T->u.str) == 0)
                semantic_error(NOT_FOUND_COL, param->T->u.str);
            if (param != 0)
            {
                for (i = 0; i < size_from; i++)
                {
                    for (j = 0; j < array_from[i].width; j++)
                    {
                        if (!strcmp(array_from[i].columns[j].name, param->T->u.str))
                            count++;
                    }
                }
                if (count > 1)
                    semantic_error(IS_STRANGE, param->T->u.str);
            }
        }
        param = 0;
        count = 0;
        get_dot(focus->L->next->T, &param);
        if (param != 0)
        {
            if ((table = getTable_from(param->L->T->T->u.str)) == 0)
                semantic_error(NOT_FOUND_TABLE, param->L->T->T->u.str);
            if (getColumn(table, param->L->next->T->T->u.str) == -1)
                semantic_error(NOT_FOUND_COL, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(focus->L->next->T, &param);
            if (param != 0 && findTable(param->T->u.str) == 0)
                semantic_error(NOT_FOUND_COL, param->T->u.str);
            if (param != 0)
            {
                for (i = 0; i < size_from; i++)
                {
                    for (j = 0; j < array_from[i].width; j++)
                    {
                        if (!strcmp(array_from[i].columns[j].name, param->T->u.str))
                            count++;
                    }
                }
                if (count > 1)
                    semantic_error(IS_STRANGE, param->T->u.str);
            }
        }
        TMP = TMP->L->T;
    }
    if (TMP != 0)
    {
        focus = TMP;
        count = 0;
        param = 0;

        get_dot(focus->L->T, &param);
        if (param != 0)
        {
            if ((table = getTable_from(param->L->T->T->u.str)) == 0)
                semantic_error(NOT_FOUND_TABLE, param->L->T->T->u.str);
            if (getColumn(table, param->L->next->T->T->u.str) == -1)
                semantic_error(NOT_FOUND_COL, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(focus->L->T, &param);
            if (param != 0 && findTable(param->T->u.str) == 0)
                semantic_error(NOT_FOUND_COL, param->T->u.str);
            if (param != 0)
            {
                for (i = 0; i < size_from; i++)
                {
                    for (j = 0; j < array_from[i].width; j++)
                    {
                        if (!strcmp(array_from[i].columns[j].name, param->T->u.str))
                            count++;
                    }
                }
                if (count > 1)
                    semantic_error(IS_STRANGE, param->T->u.str);
            }
        }
        param = 0;
        count = 0;
        get_dot(focus->L->next->T, &param);
        if (param != 0)
        {
            if ((table = getTable_from(param->L->T->T->u.str)) == 0)
                semantic_error(NOT_FOUND_TABLE, param->L->T->T->u.str);
            if (getColumn(table, param->L->next->T->T->u.str) == -1)
                semantic_error(NOT_FOUND_COL, param->L->next->T->T->u.str);
        }
        else
        {
            get_id(focus->L->next->T, &param);
            if (param != 0 && findTable(param->T->u.str) == 0)
                semantic_error(NOT_FOUND_COL, param->T->u.str);
            if (param != 0)
            {
                for (i = 0; i < size_from; i++)
                {
                    for (j = 0; j < array_from[i].width; j++)
                    {
                        if (!strcmp(array_from[i].columns[j].name, param->T->u.str))
                            count++;
                    }
                }
                if (count > 1)
                    semantic_error(IS_STRANGE, param->T->u.str);
            }
        }
        TMP = TMP->L->T;
    }
}
