#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "diff.h"

tree_t* loadMathTree(const char* filename, VariableTable* table);

node_t* getNumber();
node_t* getExpression();
node_t* getTerm();
node_t* getPrimary();
node_t* getFunction();
node_t* getGrammar();

node_t* getPower();


#endif
