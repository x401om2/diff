#include "../INCLUDES/diff.h"
#include "../INCLUDES/workWithFile.h"
#include <stdlib.h>  // calloc, free
#include <string.h>  // strcmp
#include <stdio.h>
#include <ctype.h>   // atof
#include <math.h>


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
    if (node == NULL) return;

    node->parent = parent;
    setParentLinks(node->left, node);
    setParentLinks(node->right, node);
}


void skipWhitespaces(const char* buffer, int* pos)
{
    if (buffer == NULL || pos == NULL) return;

    while (buffer[*pos] == ' ' || buffer[*pos] == '\n' || buffer[*pos] == '\t' || buffer[*pos] == '\r')
    {
        (*pos)++;
    }
}


int countTreeSize(node_t* node)
{
    if (node == NULL) return 0;
    return 1 + countTreeSize(node->left) + countTreeSize(node->right);
}


void printAkinatorTree(const node_t* node)                  // это просто для печатания дерева в терминале
{
    if (node == NULL)
    {
        printf("nil");
        return;
    }

    switch (node->type) {
        case NUM:
            printf("%.2f", node->object.constant);
            break;
        case OP:
            switch (node->object.operation) {
                case ADD:               printf("+"); break;
                case SUB:               printf("-"); break;
                case MUL:               printf("*"); break;
                case DIV:               printf("/"); break;
                case SIN:               printf("sin"); break;
                case COS:               printf("cos"); break;
                case ARCSIN:            printf("arcsin"); break;
                case ARCCOS:            printf("arccos"); break;
                case TG:                printf("tg"); break;
                case CTG:               printf("ctg"); break;
                case ARCTG:             printf("arctg"); break;
                case ARCCTG:            printf("arcctg"); break;
                case LN:                printf("ln"); break;
                case RAIZE:             printf("^"); break;
                case SQRT:              printf("sqrt"); break;
                case HZ_OPERATION:      printf("?"); break;
                case SH:                printf("sh"); break;
                case CH:                printf("ch"); break;
                case TH:                printf("th"); break;
                case CTH:               printf("cth"); break;
                default:                printf("?");
            }
            break;
        case VAR:
            printf("%s", node->object.var);
            break;
    }

    printf("(");
    printAkinatorTree(node->left);
    printf(" ");
    printAkinatorTree(node->right);
    printf(")");
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

    if (newNode == NULL) return NULL;

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


double countingTree(node_t* node, VariableTable* table)
{
    if (node == NULL) return 777;

    if (node->type == NUM)
    {
        return node->object.constant;
    }
    else if (node->type == VAR) {
        for (int i = 0; i < table->count; i++)
        {
            if (table->variables[i].isDefined && strcmp(table->variables[i].name, node->object.var) == 0)
            {
                return table->variables[i].value;
            }
        }
        printf("Ошибка: переменная %s не определена\n", node->object.var);
        return 777;
    }
    else if (node->type == OP) {
        double first = countingTree(node->left, table);
        double second = 0;

        if (node->right != NULL)
        {
            second = countingTree(node->right, table);
        }

        switch (node->object.operation) {
            case ADD: return first + second;
            case SUB: return first - second;
            case MUL: return first * second;
            case DIV:
                if (second == 0) {
                    printf("Ошибка: деление на ноль\n");
                    return 777;
                }
                return first / second;
            case SIN: return sin(first);
            case COS: return cos(first);
            case TG: return tan(first);
            case CTG:
                if (tan(first) == 0)
                {
                    printf("Ошибка: котангенс не определен\n");
                    return 777;
                }
                return 1.0 / tan(first);
            case ARCSIN:
                if (first < -1 || first > 1)
                {
                    printf("Ошибка: арксинус не определен для значения %f\n", first);
                    return 777;
                }
                return asin(first);
            case ARCCOS:
                if (first < -1 || first > 1)
                {
                    printf("Ошибка: арккосинус не определен для значения %f\n", first);
                    return 777;
                }
                return acos(first);
            case ARCTG: return atan(first);
            case ARCCTG: return PI_2 - atan(first);
            case LN:
                if (first <= 0)
                {
                    printf("Ошибка: логарифм не определен для значения %f\n", first);
                    return 777;
                }
                return log(first);
            case SQRT:
                if (first < 0)
                {
                    printf("Ошибка: квадратный корень не определен для отрицательного значения %f\n", first);
                    return 777;
                }
                return sqrt(first);
            case RAIZE:
                if (first < 0 && floor(second) != second)                                       // проверяем возведение отриц числа в дробную степень - floor функция округления вниз
                {
                    printf("Ошибка: возведение отрицательного числа в дробную степень\n");
                    return 777;
                }
                return pow(first, second);
            case SH: return sinh(first);
            case CH: return cosh(first);
            case TH: return tanh(first);
            case CTH:
                if (tanh(first) == 0)
                {
                    printf("Ошибка: гиперболический котангенс не определен\n");
                    return 777;
                }
                return 1.0 / tanh(first);
            default:
                printf("неизвестная операция");
                return 777;
        }
    }
}

void initVariableTable(VariableTable* table, int initialCapacity)
{
    table->variables = (variable_t*)calloc(initialCapacity, sizeof(variable_t));

    table->count = 0;
    table->capacity = initialCapacity;
}


variable_t* findVarInTable(VariableTable* table, char* name)                        // если не находим то создаем - нам это нужно когда мы считываем текстовик
{
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->variables[i].name, name) == 0)
        {
            return &table->variables[i];
        }
    }

    if (table->count >= table->capacity)
    {
        table->capacity *= 2;
        table->variables = (variable_t*)realloc(table->variables, table->capacity * sizeof(variable_t));
    }

    int pointer = table->count++;

    variable_t* newVariable = &table->variables[pointer];

    newVariable->name = strdup(name);
    newVariable->value = 0;
    newVariable->isDefined = false;

    return newVariable;
}


void deleteTable(VariableTable* table)
{
    for (int i = 0; i < table->count; i++)
    {
        free(table->variables[i].name);
    }

    free(table->variables);
    table->count = 0;
    table->capacity = 0;
}

