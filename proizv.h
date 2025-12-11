#ifndef PROIZV_H
#define PROIZV_H


tree_t* diffTree(const tree_t* tree);

node_t* diffNode(node_t* node);
node_t* differenciateOperation(node_t* node);

node_t* copyNode(node_t* originalNode);
bool containVariable(node_t* node);


#endif
