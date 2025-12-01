#ifndef PROIZV_H
#define PROIZV_H


tree_t* diffTree(const tree_t* tree, VariableTable* table, const char* var);

node_t* diffNode(node_t* node, const char* var, VariableTable* table);
node_t* differenciateOperation(node_t* node, VariableTable* table, const char* var);

node_t* copyNode(node_t* originalNode);
bool containVariable(node_t* node, const char* var);


#endif
