#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "diff.h"

// node_t* loadNodeMathExpressionFromBuffer(const char* buffer, int* pos, tree_t* tree, VariableTable* table);
tree_t* loadMathTree(const char* filename, VariableTable* table);

node_t* getN();
node_t* getE();
node_t* getT();
node_t* getP();
node_t* getF();
node_t* getG();


#endif
