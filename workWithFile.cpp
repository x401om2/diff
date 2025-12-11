#include "workWithFile.h"
#include "diff.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DSL.h"


// N - number ; E - expression ; T - term symbol ; Primary - выражение со скобками снаружи; F - function; Power - степень


static void skipSpacesParser(parserState* st)
{
    if (st == NULL || st->s == NULL)
    {
        return;
    }

    while (*st->s == ' ' || *st->s == '\n' || *st->s == '\t' || *st->s == '\r')
    {
        st->s++;
    }

}


node_t* getNumber(parserState* st)
{
    skipSpacesParser(st);
    double val = 0;

    while ('0' <= *st->s && *st->s <= '9')
    {
        val = (val * 10) + (*st->s - '0');
        st->s++;
    }

    skipSpacesParser(st);

    return NUM_(val);
}


node_t* getExpression(parserState* st)              // отвечает за сложение и вычитание - создает узел с двумя getTerm() если по возвращ из первой getTerm встречает знак
{
    skipSpacesParser(st);
    node_t* val = getTerm(st);

    while (*st->s == '+' || *st->s == '-')
    {
        char op = *st->s;
        st->s++;

        skipSpacesParser(st);
        node_t* val2 = getTerm(st);

        if (op == '+')
        {
            val = ADD_(val, val2);
        }
        else
        {
            val = SUB_(val, val2);
        }
    }

    skipSpacesParser(st);
    return val;
}


node_t* getTerm(parserState* st)                    // отвечает за умнож и деление
{
    skipSpacesParser(st);

    node_t* val = getPower(st);
    skipSpacesParser(st);

    while (*st->s == '*' || *st->s == '/')
    {
        char op = *st->s;
        st->s++;

        skipSpacesParser(st);

        node_t* val2 = getPower(st);

        if (op == '*')
        {
            val = MUL_(val,val2);
        }
        else
        {
            val = DIV_(val, val2);
        }
        skipSpacesParser(st);
    }

    skipSpacesParser(st);
    return val;
}


node_t* getPower(parserState* st)
{
    skipSpacesParser(st);
    node_t* val = getPrimary(st);

    skipSpacesParser(st);
    if (*st->s == '^')
    {
        st->s++;
        skipSpacesParser(st);

        node_t* exp = getPower(st);
        val = POW_(val, exp);
    }
    skipSpacesParser(st);

    return val;
}



node_t* getFunction(parserState* st)                                  // отвечает за индикаторы и функции
{
    skipSpacesParser(st);

    char funcName[MAX_NAME_LEN] = {0};
    int i = 0;

    while (isalpha(*st->s))
    {
        funcName[i++] = *st->s;
        st->s++;
    }
    funcName[i] = '\0';
    skipSpacesParser(st);

    if (strcmp(funcName, "sin") == 0 || strcmp(funcName, "cos") == 0 ||
        strcmp(funcName, "tg") == 0 || strcmp(funcName, "ctg") == 0 ||
        strcmp(funcName, "arcsin") == 0 || strcmp(funcName, "arccos") == 0 ||
        strcmp(funcName, "arctg") == 0 || strcmp(funcName, "arcctg") == 0 ||
        strcmp(funcName, "ln") == 0 || strcmp(funcName, "sqrt") == 0 ||
        strcmp(funcName, "sh") == 0 || strcmp(funcName, "ch") == 0 ||
        strcmp(funcName, "th") == 0 || strcmp(funcName, "cth") == 0)

    {
        if (*st->s == '(')
        {
            st->s++;
            skipSpacesParser(st);

            node_t* arg = getExpression(st);                                                // после того как вернемся из E - будет ждать ')' - и создаст узел с полученной операцией

            skipSpacesParser(st);
            if (*st->s == ')')
            {
                st->s++;
            }
            else
            {
                printf("ожидалась ')' после %s\n", funcName);
            }
            return createTypedNode(OP, funcName, arg, NULL);
        }
    }

    // либо если не операция - получается на вход поступила переменная

    return VAR_("x");
}


node_t* getPrimary(parserState* st)                                      // отвечает за первичные выражения
{
    skipSpacesParser(st);

    if (*st->s == '-')
    {
        st->s++;
        skipSpacesParser(st);

        node_t* val = getPrimary(st);
        return MUL_(NUM_(-1), val);
    }

    if (*st->s == '+')
    {
        st->s++;
        skipSpacesParser(st);
        return getPrimary(st);
    }

    if (*st->s == '(')
    {
        st->s++;
        skipSpacesParser(st);

        node_t* val = getExpression(st);

        skipSpacesParser(st);
        if (*st->s == ')')
        {
            st->s++;
        }
        else
        {
            printf("аhtung: ожидалась ')'\n");
        }
        return val;
    }
    else if (isalpha(*st->s))
    {
        return getFunction(st);
    }
    else
    {
        return getNumber(st);
    }
}


node_t* getGrammar(parserState* st)                                                        // главная функция вызывающая - general
{
    skipSpacesParser(st);
    node_t* val = getExpression(st);

    skipSpacesParser(st);

    if (*st->s != '$' && *st->s != '\0')
    {
        printf("ваырнинг бляттть: неожиданный символ '%c' в конце\n", *st->s);
    }

    return val;
}


tree_t* loadMathTree(const char* filename)                                              //VariableTable* table
{
    FILE* file = fopen(filename, "r");

    if (!file)
    {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)calloc(file_size + 2, sizeof(char));
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    fclose(file);

    if (bytes_read != file_size)
    {
        printf("ашибка чтения файла\n");
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';


    parserState st = {
        .s = buffer,
    };

    tree_t* tree = treeCtor();

    if (tree == NULL)
    {
        printf("еррор емае не удалось создать дерево\n");
        free(buffer);
        return NULL;
    }

    tree->root = getGrammar(&st);

    if (tree->root == NULL)
    {
        printf("ошибочка не удалось распарсить выражение\n");
        free(buffer);
        free(tree);
        return NULL;
    }

    free(buffer);

    setParentLinks(tree->root, NULL);
    tree->size = countTreeSize(tree->root);

    return tree;
}

#include "DSL_undef.h"
