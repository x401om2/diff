#ifndef DIFF_H
#define DIFF_H

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

const int COUNT_OF_VARIABLES = 3;
const int MAX_NAME_LEN = 200;
const int DATA_CAPACITY = 100;
const int MAX_INPUT_LEN = 50;
const int MAXIMUS_LEN = 300;

const double PI_2 = 1.570796;


typedef enum
{
    SUCCESS = 0,
    ERROR_NULL_POINTER = 1,
    ERROR_INVALID_SIZE = 2,
    ERROR_SYNTAX = 3,
    ERROR_INVALID_OPERATION = 4,
    BAD_OPERATION_TYPE = 7

} errors_t;


typedef enum {
    TREE_SUCCESS = 0,
    TREE_ERROR_NULL_PTR = 1,
    TREE_ERROR_INVALID = 2
} treeError_t;


enum operations_t                   // типы операций которые доступны
{
    HZ_OPERATION = -1,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    SIN = 5,
    COS = 6,
    TG = 7,
    CTG = 8,
    ARCSIN = 9,
    ARCCOS = 10,
    ARCTG = 11,
    ARCCTG = 12,
    LN = 13,
    RAIZE = 14,
    SQRT = 15,
    SH = 16,    // гиперболический синус
    CH = 17,    // гиперболический косинус
    TH = 18,    // гиперболический тангенс
    CTH = 19    // гиперболический котангенс
};

union object_t                               // как дед говорил в union тип заключить типы которые могут быть у узла
{
    double constant;                         // для чисел
    operations_t operation;                  // для операций
    char* var;                               // для переменных
};

enum type_t                          // тип узла
{
    NUM = 1,
    OP = 2,
    VAR = 3,
};

typedef struct node_t node_t;

struct node_t {
    type_t type;
    object_t object;
    node_t* left;
    node_t* right;
    node_t* parent;
};


typedef struct {
    node_t* root;
    size_t size;
} tree_t;


typedef struct {
    char* name;
    double value;
    bool isDefined;
} variable_t;


typedef struct {
    variable_t* variables;
    int count;
    int capacity;
} VariableTable;


tree_t* treeCtor(void);
errors_t treeRecursiveDelete(node_t* node);


void setParentLinks(node_t* node, node_t* parent);
void skipWhitespaces(const char* buffer, int* pos);
int countTreeSize(node_t* node);

void printAkinatorTree(const node_t* node);

void deleteTable(VariableTable* table);

node_t* createTypedNode(type_t type, const char* data, node_t* leftNode, node_t* rightNode);

node_t* createNumNode(double value);

double countingTree(node_t* node, VariableTable* table);

variable_t* findVarInTable(VariableTable* table, char* name);

void initVariableTable(VariableTable* table, int initialCapacity);

#endif
