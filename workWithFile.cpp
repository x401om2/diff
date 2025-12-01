#include "workWithFile.h"
#include "diff.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* s = NULL;
static VariableTable* current_table = NULL;

// N - number ; E - expression ; T - term symbol ; P - выражение со скобками снаружи; F - function

node_t* getN()
{
    double val = 0;

    while ('0' <= *s && *s <= '9')
    {
        val = (val * 10) + (*s - '0');
        s++;
    }

    return createNumNode(val);
}

node_t* getE()                      // отвечает за сложение и вычитание
{
    node_t* val = getT();

    while (*s == '+' || *s == '-')
    {
        char op = *s;
        s++;
        node_t* val2 = getT();
        if (op == '+')
        {
            val = createTypedNode(OP, "+", val, val2);
        }
        else {
            val = createTypedNode(OP, "-", val, val2);
        }
    }
    return val;
}

node_t* getT()                              // отвечает за умножение и деление
{
    node_t* val = getP();

    while (*s == '*' || *s == '/')
    {
        char op = *s;
        s++;
        node_t* val2 = getP();
        if (op == '*')
        {
            val = createTypedNode(OP, "*", val, val2);
        }
        else {
            val = createTypedNode(OP, "/", val, val2);
        }
    }
    return val;
}

node_t* getF()                                  // отвечает за индикаторы и функции
{
    char funcName[MAX_NAME_LEN] = {0};
    int i = 0;

    while (isalpha(*s))
    {
        funcName[i++] = *s;
        s++;
    }
    funcName[i] = '\0';

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
            node_t* arg = getE();
            if (*s == ')')
            {
                s++;
            } else {
                printf("ожидалась ')' после %s\n", funcName);
            }
            return createTypedNode(OP, funcName, arg, NULL);
        }
    }

    findVarInTable(current_table, funcName);

    return createTypedNode(VAR, funcName, NULL, NULL);
}

node_t* getP()                                      // отвечает за первичные выражения
{
    if (*s == '(')
    {
        s++;
        node_t* val = getE();
        if (*s == ')')
        {
            s++;
        } else {
            printf("Ошибка: ожидалась ')'\n");
        }
        return val;
    }
    else if (isalpha(*s))
    {
        return getF();
    }
    else
    {
        return getN();
    }
}

node_t* getG()                  // главная функция вызывающая
{
    node_t* val = getE();

    if (*s != '$' && *s != '\0')
    {
        printf("Ошибка: неожиданный символ '%c' в конце\n", *s);
    }

    return val;
}

tree_t* loadMathTree(const char* filename, VariableTable* table)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
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

    s = buffer;
    current_table = table;

    tree_t* tree = treeCtor();
    if (tree == NULL)
    {
        printf("Ошибка: не удалось создать дерево\n");
        free(buffer);
        return NULL;
    }

    tree->root = getG();

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

