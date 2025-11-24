#include <stdio.h>
#include "diff.h"
#include "dump.h"
#include "proizv.h"


tree_t* diffTree(const tree_t* tree, VariableTable* table, const char* var)
{
    tree_t* afterDiffTree = treeCtor();

    afterDiffTree->root = diffNode(tree->root, var, table);

    return afterDiffTree;
}



node_t* diffNode(node_t* node, const char* var, VariableTable* table)
{
    // проверки

    switch (node->type) {
        case NUM:
            return createTypedNode(NUM, "0", NULL, NULL);

        case VAR:
            if (strcmp(node->object.var, var) == 0)
            {
                return createTypedNode(NUM, "1", NULL, NULL);
            } else {
                return createTypedNode(NUM, "0", NULL, NULL);
            }

        case OP:
            return differenciateOperation(node, table, var);
    }

}




node_t* differenciateOperation(node_t* node, VariableTable* table, const char* var)
{
    // проверки
    switch (node->object.operation) {
        case ADD:
            {
                node_t* newNode = createTypedNode(OP, "+", diffNode(node->left, var, table), diffNode(node->right, var, table));
                return newNode;
            }
        case SUB:
            {
                node_t* newNode = createTypedNode(OP, "-", diffNode(node->left, var, table), diffNode(node->right, var, table));
                return newNode;
            }
        case MUL:
            {
                node_t* leftMul = createTypedNode(OP, "*", diffNode(node->left, var, table), copyNode(node->right));

                node_t* rightMul = createTypedNode(OP, "*", copyNode(node->left), diffNode(node->right, var, table));

                node_t* addNode = createTypedNode(OP, "+", leftMul, rightMul);


                return addNode;
            }
        case RAIZE:
            {
                bool varInLeft = containVariable(node->left, var);
                bool varInRight = containVariable(node->right, var);

                if (varInLeft && !varInRight)
                {
                    // случай когда x в основании u^a
                    node_t* a_node = copyNode(node->right);
                    node_t* u_node = copyNode(node->left);

                    node_t* aMinusOne = createTypedNode(OP, "-", copyNode(node->right), createTypedNode(NUM, "1", NULL, NULL));

                    node_t* U_pow_AminusOne = createTypedNode(OP, "^", u_node, aMinusOne);

                    node_t* diffU = diffNode(u_node, var, table);

                    node_t* firstMul = createTypedNode(OP, "*", a_node, U_pow_AminusOne);
                    node_t* result = createTypedNode(OP, "*", firstMul, diffU);

                    return result;

                } else if (!varInLeft && varInRight) {
                    // случай когда х в степени a^u

                    node_t* a_node = copyNode(node->left);
                    node_t* u_node = copyNode(node->right);

                    node_t* ln_a = createTypedNode(OP, "ln", a_node, NULL);
                    node_t* pow_u = diffNode(u_node, var, table);

                    node_t* firstMul = createTypedNode(OP, "*", node, ln_a);
                    node_t* result = createTypedNode(OP, "*", firstMul, pow_u);

                    return result;

                } else if (varInLeft && varInRight) {
                    // случай когда х и в степени и в основании



                }
            }
    }
}


node_t* copyNode(node_t* originalNode)
{
    // проверки - ассерты стоит
    node_t* newNode = NULL;

    const char* nodeValue = getNodeValueString(originalNode);

    switch (originalNode->type) {
        case OP:
            newNode = createTypedNode(OP, nodeValue, copyNode(originalNode->left), copyNode(originalNode->right));
            break;
        case NUM:
            newNode = createTypedNode(NUM, nodeValue, NULL, NULL);
            break;
        case VAR:
            newNode = createTypedNode(VAR, nodeValue, NULL, NULL);
            break;
    }
    return newNode;
}


bool containVariable(node_t* node, const char* var)
{
    if (node == NULL) return false;

    if (node->type == VAR && strcmp(node->object.var, var) == 0)
    {
        return true;
    }

    return containVariable(node->left, var) || containVariable(node->right, var);
}
