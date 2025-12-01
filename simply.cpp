#include <stdio.h>
#include "diff.h"
#include "toLatex.h"
#include "proizv.h"
#include "dump.h"
#include <math.h>


node_t* simplification(node_t* node, tree_t* originalTree, FILE* texFile)
{
    if (node == NULL) return NULL;

    const char* title = "Simplification step";

    node->left = simplification(node->left, originalTree, texFile);
    node->right = simplification(node->right, originalTree, texFile);

    node_t* original_node = copyNode(node);

    node_t* result = node; // по умолчанию не меняем

    if (node->type == OP)
    {
        if (node->object.operation == ADD)
        {
            if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
            {
                result = createNumNode(node->left->object.constant + node->right->object.constant);
            }
            else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
            {
                result = copyNode(node->right);
            }
            else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
            {
                result = copyNode(node->left);
            }
        }

        else if (node->object.operation == SUB)
        {
            if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
            {
                result = createNumNode(node->left->object.constant - node->right->object.constant);
            }
            else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
            {
                result = copyNode(node->left);
            }
            else if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->left->object.constant == 0)
            {
                result = createTypedNode(OP, "*", createNumNode(-1), copyNode(node->right));
            }
        }

        else if (node->object.operation == MUL)
        {
            if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
            {
                result = createNumNode(node->left->object.constant * node->right->object.constant);
            }
            else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
            {
                result = createNumNode(0);
            }
            else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
            {
                result = createNumNode(0);
            }
            else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 1)
            {
                result = copyNode(node->right);
            }
            else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 1)
            {
                result = copyNode(node->left);
            }
        }

        else if (node->object.operation == DIV)
        {
            if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
            {
                result = createNumNode(node->left->object.constant / node->right->object.constant);
            }
            else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
            {
                result = createNumNode(0);
            }
            else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 1)
            {
                result = copyNode(node->left);
            }
        }

        else if (node->object.operation == RAIZE)
        {
            if (node->left != NULL && node->right != NULL && node->left->type == NUM && node->right->type == NUM)
            {
                result = createNumNode(pow(node->left->object.constant, node->right->object.constant));
            }
            else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 1)
            {
                result = copyNode(node->left);
            }
            else if (node->right != NULL && node->right->type == NUM && node->right->object.constant == 0)
            {
                result = createNumNode(1);
            }
            else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 1)
            {
                result = createNumNode(1);
            }
            else if (node->left != NULL && node->left->type == NUM && node->left->object.constant == 0)
            {
                result = createNumNode(0);
            }
        }
    }

    if (result != node && texFile != NULL)
    {
        formulaToLatex(originalTree, texFile, title);

        treeRecursiveDelete(node);

        return result;
    }
    // в result я записываю изменненный узел - если он не эквивалентен тому что было в начале то выводим - так как изменился

    treeRecursiveDelete(original_node);
    return node;
}
