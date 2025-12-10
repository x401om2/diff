#ifndef SIMPLY_H
#define SIMPLY_H

#include <stdio.h>

node_t* simplification(node_t* node, tree_t* originalTree, FILE* texFile);
bool bothAreNums(node_t* left, node_t* right);
bool isOne(node_t* node);
bool isZero(node_t* node);

node_t* simplifyAdd(node_t* node);
node_t* simplifySub(node_t* node);
node_t* simplifyMul(node_t* node);
node_t* simplifyDiv(node_t* node);
node_t* simplifyRaize(node_t* node);

#endif
