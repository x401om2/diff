#include <stdio.h>
#include "../INCLUDES/diff.h"
#include "../INCLUDES/toLatex.h"
#include "../INCLUDES/proizv.h"
#include "../INCLUDES/dump.h"
#include <math.h>
#include "../INCLUDES/DSL.h"


node_t* simplification(node_t* node, tree_t* originalTree, FILE* texFile)
{
    if (node == NULL) return NULL;

    const char* title = "шажочек упрощения :)";

    node->left = simplification(node->left, originalTree, texFile);
    node->right = simplification(node->right, originalTree, texFile);

    node_t* originalNode = copyNode(node);
    node_t* result = node;  // по умолчанию не меняем

    if (node->type == OP)
    {
        switch (node->object.operation) {
            case ADD:
                // 2 + 3 = 5
                if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
                {
                    result = NUM_(node->left->object.constant + node->right->object.constant);
                }
                // 0 + x = x
                else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
                {
                    result = copyNode(node->right);
                }
                // x + 0 = x
                else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
                {
                    result = copyNode(node->left);
                }
                break;

            case SUB:
                // 5 - 3 = 2
                if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
                {
                    result = NUM_(node->left->object.constant - node->right->object.constant);
                }
                // x - 0 = x
                else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
                {
                    result = copyNode(node->left);
                }
                // 0 - x = -x
                else if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->left->object.constant == 0)
                {
                    result = MUL_(NUM_(-1), copyNode(node->right));
                }
                break;

            case MUL:
                // 2 * 3 = 6
                if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
                {
                    result = NUM_(node->left->object.constant * node->right->object.constant);
                }
                // 0 * x = 0
                else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
                {
                    result = NUM_(0);
                }
                // x * 0 = 0
                else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
                {
                    result = NUM_(0);
                }
                // 1 * x = x
                else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 1)
                {
                    result = copyNode(node->right);
                }
                // x * 1 = x
                else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 1)
                {
                    result = copyNode(node->left);
                }
                break;

            case DIV:
                // 6 / 3 = 2
                if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
                {
                    result = NUM_(node->left->object.constant / node->right->object.constant);
                }
                // 0 / x = 0
                else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
                {
                    result = NUM_(0);
                }
                // x / 1 = x
                else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 1)
                {
                    result = copyNode(node->left);
                }
                break;

            case RAIZE:
                // 2^3 = 8
                if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
                {
                    result = NUM_(pow(node->left->object.constant, node->right->object.constant));
                }
                // x^1 = x
                else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 1)
                {
                    result = copyNode(node->left);
                }
                // x^0 = 1
                else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
                {
                    result = NUM_(1);
                }
                // 1^x = 1
                else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 1)
                {
                    result = NUM_(1);
                }
                // 0^x = 0
                else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
                {
                    result = NUM_(0);
                }
                break;

            default:
                break;
        }
    }
    // если узел изменился то выводим егогг
    if (result != node && texFile != NULL)
    {
        formulaToLatex(originalTree, texFile, title);
        treeRecursiveDelete(node);
        treeRecursiveDelete(originalNode);
        return result;
    }

    treeRecursiveDelete(originalNode);
    return node;
}
