#include "diff.h"
#include "workWithFile.h"
#include <stdlib.h>  // calloc, free
#include <string.h>  // strcmp
#include <stdio.h>
#include <ctype.h>   // atof
#include <math.h>
#include "DSL.h"


tree_t* treeCtor(void)
{
    tree_t* tree = (tree_t*)calloc(1, sizeof(tree_t));

    if (tree != NULL)
    {
        tree->size = 0;
        tree->root = NULL;
    }
    return tree;
}


errors_t treeRecursiveDelete(node_t* node)
{
    if (node == NULL)
    {
        return SUCCESS;
    }

    treeRecursiveDelete(node->left);
    treeRecursiveDelete(node->right);

    free(node);

    return SUCCESS;
}



void setParentLinks(node_t* node, node_t* parent)
{
    if (node == NULL)
    {
        return;
    }

    node->parent = parent;
    setParentLinks(node->left, node);
    setParentLinks(node->right, node);
}


void skipWhitespaces(const char* buffer, int* pos)
{
    if (buffer == NULL || pos == NULL)
    {
        printf("ошибка нулевой указатель передан в функции пропуска пробелов\n");
        return;
    }

    while (buffer[*pos] == ' ' || buffer[*pos] == '\n' || buffer[*pos] == '\t' || buffer[*pos] == '\r')
    {
        (*pos)++;
    }
}


int countTreeSize(node_t* node)
{
    if (node == NULL)
    {
        return 0;
    }
    return 1 + countTreeSize(node->left) + countTreeSize(node->right);
}


node_t* createNumNode(double value)
{
    node_t* newNode = (node_t*)calloc(1, sizeof(node_t));

    newNode->type = NUM;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->object.constant = value;

    return newNode;
}

node_t* createTypedNode(type_t type, const char* data, node_t* leftNode, node_t* rightNode)
{
    node_t* newNode = (node_t*)calloc(1, sizeof(node_t));

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->type = type;

    switch (type) {
        case OP:
            if (strcmp("+",data) == 0)              newNode->object.operation = ADD;
            else if (strcmp("-",data) == 0)         newNode->object.operation = SUB;
            else if (strcmp("*",data) == 0)         newNode->object.operation = MUL;
            else if (strcmp("/",data) == 0)         newNode->object.operation = DIV;
            else if (strcmp("sin",data) == 0)       newNode->object.operation = SIN;
            else if (strcmp("arcsin",data) == 0)    newNode->object.operation = ARCSIN;
            else if (strcmp("cos",data) == 0)       newNode->object.operation = COS;
            else if (strcmp("arccos",data) == 0)    newNode->object.operation = ARCCOS;
            else if (strcmp("tg",data) == 0)        newNode->object.operation = TG;
            else if (strcmp("arctg",data) == 0)     newNode->object.operation = ARCTG;
            else if (strcmp("ctg",data) == 0)       newNode->object.operation = CTG;
            else if (strcmp("arcctg",data) == 0)    newNode->object.operation = ARCCTG;
            else if (strcmp("ln",data) == 0)        newNode->object.operation = LN;
            else if (strcmp("^",data) == 0)         newNode->object.operation = RAIZE;
            else if (strcmp("sqrt",data) == 0)      newNode->object.operation = SQRT;
            else if (strcmp("sh",data) == 0)        newNode->object.operation = SH;
            else if (strcmp("ch",data) == 0)        newNode->object.operation = CH;
            else if (strcmp("th",data) == 0)        newNode->object.operation = TH;
            else if (strcmp("cth",data) == 0)       newNode->object.operation = CTH;
            else newNode->object.operation = HZ_OPERATION;
            break;
        case VAR:
            if (data[0] != '\0') newNode->object.var = strdup(data);
            break;
    }

    newNode->left = leftNode;
    newNode->right = rightNode;
    newNode->parent = NULL;

    return newNode;
}

// double countingTree(node_t* node, VariableTable* table)
// {
//     if (node == NULL)
//     {
//         printf("ошибка - указатель на node == NULL - функция countingTree\n");
//     }
//
//     if (node->type == NUM)
//     {
//         return node->object.constant;
//     }
//     else if (node->type == VAR)
//     {
//         for (int i = 0; i < table->count; i++)                                                                      // ищем переменную по имени в таблице переменных
//         {
//             if (table->variables[i].isDefined && strcmp(table->variables[i].name, node->object.var) == 0)
//             {
//                 return table->variables[i].value;
//             }
//         }
//         printf("ahtuing: переменная %s не определена\n", node->object.var);
//         return NAN;
//     }
//     else if (node->type == OP)
//     {
//         double first = countingTree(node->left, table);                         // считаем левое поддерево
//         if (isnan(first))
//         {
//             return NAN;
//         }
//
//         double second = 0;
//         if (node->right != NULL)                                                // если существует правое поддерево то считаем и его
//         {
//             second = countingTree(node->right, table);
//             if (isnan(second))
//             {
//                 return NAN;
//             }
//         }
//
//         switch (node->object.operation) {                                       // ну тут в зависимости от операции возвращаем значения поддеревьев под функциями
//             case ADD: return first + second;
//             case SUB: return first - second;
//             case MUL: return first * second;
//             case DIV:
//                 if (second == 0)
//                 {
//                     printf("ахтунг опасный: деление на ноль\n");
//                     return NAN;
//                 }
//                 return first / second;
//             case SIN: return sin(first);
//             case COS: return cos(first);
//             case TG: return tan(first);
//             case CTG:
//                 if (tan(first) == 0)
//                 {
//                     printf("ахтунг: котангенс не определен\n");
//                     return NAN;
//                 }
//                 return 1.0 / tan(first);
//             case ARCSIN:
//                 if (first < -1 || first > 1)
//                 {
//                     printf("ахтунг: арксинус не определен для значения %f\n", first);
//                     return NAN;
//                 }
//                 return asin(first);
//             case ARCCOS:
//                 if (first < -1 || first > 1)
//                 {
//                     printf("емае ахтунг: арккосинус не определен для значения %f\n", first);
//                     return NAN;
//                 }
//                 return acos(first);
//             case ARCTG: return atan(first);
//             case ARCCTG: return PI_2 - atan(first);
//             case LN:
//                 if (first <= 0)
//                 {
//                     printf("ашибка: логарифм не определен для значения %f\n", first);
//                     return NAN;
//                 }
//                 return log(first);
//             case SQRT:
//                 if (first < 0)
//                 {
//                     printf("ашебка: квадратный корень не определен для отрицательного значения %f\n", first);
//                     return NAN;
//                 }
//                 return sqrt(first);
//             case RAIZE:
//                 if (first < 0 && floor(second) != second)                                       // проверяем возведение отриц числа в дробную степень - floor функция округления вниз
//                 {
//                     printf("абобус блттт: возведение отрицательного числа в дробную степень\n");
//                     return NAN;
//                 }
//                 return pow(first, second);
//             case SH: return sinh(first);
//             case CH: return cosh(first);
//             case TH: return tanh(first);
//             case CTH:
//                 if (tanh(first) == 0)
//                 {
//                     printf("фак ошибка: гиперболический котангенс не определен\n");
//                     return NAN;
//                 }
//                 return 1.0 / tanh(first);
//             default:
//                 printf("хер знает брад операция мне не ясна\n");
//                 return NAN;
//         }
//     }
// }


double countingTree(node_t* node, double valueOfX)
{
    if (node == NULL)
    {
        printf("ошибка - указатель на node == NULL - функция countingTree\n");
    }

    if (node->type == NUM)
    {
        return node->object.constant;
    }
    else if (node->type == VAR)
    {
        if (strcmp(node->object.var, "x") == 0)
        {
            return valueOfX;
        }
    }
    else if (node->type == OP)
    {
        double first = countingTree(node->left, valueOfX);                         // считаем левое поддерево
        if (isnan(first))
        {
            return NAN;
        }

        double second = 0;
        if (node->right != NULL)                                                // если существует правое поддерево то считаем и его
        {
            second = countingTree(node->right, valueOfX);
            if (isnan(second))
            {
                return NAN;
            }
        }

        switch (node->object.operation) {                                       // ну тут в зависимости от операции возвращаем значения поддеревьев под функциями
            case ADD: return first + second;
            case SUB: return first - second;
            case MUL: return first * second;
            case DIV:
                if (second == 0)
                {
                    printf("ахтунг опасный: деление на ноль\n");
                    return NAN;
                }
                return first / second;
            case SIN: return sin(first);
            case COS: return cos(first);
            case TG: return tan(first);
            case CTG:
                if (tan(first) == 0)
                {
                    printf("ахтунг: котангенс не определен\n");
                    return NAN;
                }
                return 1.0 / tan(first);
            case ARCSIN:
                if (first < -1 || first > 1)
                {
                    printf("ахтунг: арксинус не определен для значения %f\n", first);
                    return NAN;
                }
                return asin(first);
            case ARCCOS:
                if (first < -1 || first > 1)
                {
                    printf("емае ахтунг: арккосинус не определен для значения %f\n", first);
                    return NAN;
                }
                return acos(first);
            case ARCTG: return atan(first);
            case ARCCTG: return PI_2 - atan(first);
            case LN:
                if (first <= 0)
                {
                    printf("ашибка: логарифм не определен для значения %f\n", first);
                    return NAN;
                }
                return log(first);
            case SQRT:
                if (first < 0)
                {
                    printf("ашебка: квадратный корень не определен для отрицательного значения %f\n", first);
                    return NAN;
                }
                return sqrt(first);
            case RAIZE:
                if (first < 0 && floor(second) != second)                                       // проверяем возведение отриц числа в дробную степень - floor функция округления вниз
                {
                    printf("абобус блттт: возведение отрицательного числа в дробную степень\n");
                    return NAN;
                }
                return pow(first, second);
            case SH: return sinh(first);
            case CH: return cosh(first);
            case TH: return tanh(first);
            case CTH:
                if (tanh(first) == 0)
                {
                    printf("фак ошибка: гиперболический котангенс не определен\n");
                    return NAN;
                }
                return 1.0 / tanh(first);
            default:
                printf("хер знает брад операция мне не ясна\n");
                return NAN;
        }
    }
}




void getVariableValues(double* x)
{
    printf("введите значение для x: ");

    if (scanf("%lf", &x) != 1)
    {
        printf("неверно ввели - по умолчанию поставим 0\n");
        *x = 0;
    }
}



void getPlotRangeFromUser(float* minX, float* maxX)                                 // получаем диапазон иксов у пользователя
{
    float defaultMin = -5.0;
    float defaultMax = 5.0;

    printf("диапазон построения графиков:\n");
    printf("введите минимальный x (по умолчанию поставим - %.2f): ", defaultMin);

    if (scanf("%f", minX) != 1)
    {
        *minX = defaultMin;
    }

    printf("введите максимальный x (по умолчанию поставим - %.2f): ", defaultMax);

    if (scanf("%f", maxX) != 1)
    {
        *maxX = defaultMax;
    }

    if (*minX >= *maxX)
    {
        printf("ахтунг: минимальное значение должно быть меньше максимального\n");
        printf("используем значение по умолчанию (-5, 5)\n");
        *minX = defaultMin;
        *maxX = defaultMax;
    }
}

#include "DSL_undef.h"
