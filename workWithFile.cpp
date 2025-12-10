#include "workWithFile.h"
#include "diff.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DSL.h"


static const char* s = NULL;
static VariableTable* current_table = NULL;

// N - number ; E - expression ; T - term symbol ; Primary - выражение со скобками снаружи; F - function; Power - степень

static void skipSpacesParser()
{
    if (s == NULL) return;

    while (*s == ' ' || *s == '\n' || *s == '\t' || *s == '\r')
    {
        s++;
    }
}


node_t* getNumber()
{
    skipSpacesParser();
    double val = 0;

    while ('0' <= *s && *s <= '9')
    {
        val = (val * 10) + (*s - '0');
        s++;
    }

    skipSpacesParser();

    return NUM_(val);
}


node_t* getExpression()                                             // отвечает за сложение и вычитание
{
    skipSpacesParser();
    node_t* val = getTerm();

    while (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;

        skipSpacesParser();

        node_t* val2 = getTerm();
        if (op == '+')
        {
            val = ADD_(val, val2);
        }
        else
        {
            val = SUB_(val, val2);
        }
    }

    skipSpacesParser();
    return val;
}


node_t* getTerm()                                                   // отвечает за умножение и деление
{
    skipSpacesParser();
    node_t* val = getPower();

    skipSpacesParser();
    while (*s == '*' || *s == '/')
    {
        char op = *s;
        s++;

        skipSpacesParser();
        node_t* val2 = getPower();
        if (op == '*')
        {
            val = MUL_(val, val2);
        }
        else
        {
            val = DIV_(val, val2);
        }
        skipSpacesParser();
    }

    skipSpacesParser();
    return val;
}


node_t* getPower()
{
    skipSpacesParser();
    node_t* val = getPrimary();

    skipSpacesParser();
    if (*s == '^')
    {
        s++;
        skipSpacesParser();

        node_t* exponent = getPower();
        val = POW_(val, exponent);
    }

    skipSpacesParser();
    return val;
}


node_t* getFunction()                                  // отвечает за индикаторы и функции
{
    skipSpacesParser();

    char funcName[MAX_NAME_LEN] = {0};
    int i = 0;

    while (isalpha(*s))
    {
        funcName[i++] = *s;
        s++;
    }
    funcName[i] = '\0';
    skipSpacesParser();

    if (strcmp(funcName, "sin") == 0 || strcmp(funcName, "cos") == 0 ||
        strcmp(funcName, "tg") == 0 || strcmp(funcName, "ctg") == 0 ||
        strcmp(funcName, "arcsin") == 0 || strcmp(funcName, "arccos") == 0 ||
        strcmp(funcName, "arctg") == 0 || strcmp(funcName, "arcctg") == 0 ||
        strcmp(funcName, "ln") == 0 || strcmp(funcName, "sqrt") == 0 ||
        strcmp(funcName, "sh") == 0 || strcmp(funcName, "ch") == 0 ||
        strcmp(funcName, "th") == 0 || strcmp(funcName, "cth") == 0)

    {

        if (*s == '(')
        {
            s++;
            skipSpacesParser();

            node_t* arg = getExpression();

            skipSpacesParser();
            if (*s == ')')
            {
                s++;
            }
            else
            {
                printf("ожидалась ')' после %s\n", funcName);
            }
            return createTypedNode(OP, funcName, arg, NULL);
        }
    }

    findVarInTable(current_table, funcName);

    return VAR_(funcName);
}


node_t* getPrimary()                                      // отвечает за первичные выражения
{
    skipSpacesParser();

    if (*s == '-')
    {
        s++;
        skipSpacesParser();

        node_t* val = getPrimary();
        return MUL_(NUM_(-1), val);
    }

    if (*s == '+')
    {
        s++;
        skipSpacesParser();
        return getPrimary();
    }

    if (*s == '(')
    {
        s++;
        skipSpacesParser();

        node_t* val = getExpression();

        skipSpacesParser();
        if (*s == ')')
        {
            s++;
        }
        else
        {
            printf("аhtung: ожидалась ')'\n");
        }
        return val;
    }
    else if (isalpha(*s))
    {
        return getFunction();
    }
    else
    {
        return getNumber();
    }
}


node_t* getGrammar()                                                        // главная функция вызывающая - general
{
    skipSpacesParser();
    node_t* val = getExpression();

    skipSpacesParser();

    if (*s != '$' && *s != '\0')
    {
        printf("ваырнинг бляттть: неожиданный символ '%c' в конце\n", *s);
    }

    return val;
}


tree_t* loadMathTree(const char* filename, VariableTable* table)
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
        printf("Ошибка чтения файла\n");
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';

    s = buffer;                             // как раз то что сверху задали static const - изменяем здесь
    current_table = table;

    tree_t* tree = treeCtor();

    if (tree == NULL)
    {
        printf("Ошибка: не удалось создать дерево\n");
        free(buffer);
        return NULL;
    }

    tree->root = getGrammar();

    if (tree->root == NULL)
    {
        printf("Ошибка: не удалось распарсить выражение\n");
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
