#include <stdio.h>
#include "diff.h"
#include "simply.h"
#include "toLatex.h"
#include "proizv.h"
#include "dump.h"
#include <math.h>
#include "DSL.h"



// node_t* simplification(node_t* node, tree_t* originalTree, FILE* texFile)
// {
//     if (node == NULL)
//     {
//         return NULL;
//     }
//
//     const char* title = "шаг упрощения:";
//
//     node->left = simplification(node->left, originalTree, texFile);
//     node->right = simplification(node->right, originalTree, texFile);
//
//     node_t* originalNode = copyNode(node);
//     node_t* result = node;  // по умолчанию не меняем
//
//     if (node->type == OP)
//     {
//         switch (node->object.operation) {
//             case ADD:
//                 // 2 + 3 = 5
//                 if (bothAreNums(node->left, node->right))
//                 {
//                     result = NUM_(node->left->object.constant + node->right->object.constant);
//                 }
//                 // 0 + x = x
//                 else if (isZero(node->left))
//                 {
//                     result = copyNode(node->right);
//                 }
//                 // x + 0 = x
//                 else if (isZero(node->right))
//                 {
//                     result = copyNode(node->left);
//                 }
//                 break;
//
//             case SUB:
//                 // 5 - 3 = 2
//                 if (bothAreNums(node->left, node->right))
//                 {
//                     result = NUM_(node->left->object.constant - node->right->object.constant);
//                 }
//                 // x - 0 = x
//                 else if (isZero(node->right))
//                 {
//                     result = copyNode(node->left);
//                 }
//                 // 0 - x = -x
//                 else if (isZero(node->left))
//                 {
//                     result = MUL_(NUM_(-1), copyNode(node->right));
//                 }
//                 break;
//
//             case MUL:
//                 // 2 * 3 = 6
//                 if (bothAreNums(node->left, node->right))
//                 {
//                     result = NUM_(node->left->object.constant * node->right->object.constant);
//                 }
//                 // 0 * x = 0
//                 else if (isZero(node->left))
//                 {
//                     result = NUM_(0);
//                 }
//                 // x * 0 = 0
//                 else if (isZero(node->right))
//                 {
//                     result = NUM_(0);
//                 }
//                 // 1 * x = x
//                 else if (isOne(node->left))
//                 {
//                     result = copyNode(node->right);
//                 }
//                 // x * 1 = x
//                 else if (isOne(node->right))
//                 {
//                     result = copyNode(node->left);
//                 }
//                 break;
//
//             case DIV:
//                 // 6 / 3 = 2
//                 if (bothAreNums(node->left, node->right))
//                 {
//                     result = NUM_(node->left->object.constant / node->right->object.constant);
//                 }
//                 // 0 / x = 0
//                 else if (isZero(node->left))
//                 {
//                     result = NUM_(0);
//                 }
//                 // x / 1 = x
//                 else if (isOne(node->right))
//                 {
//                     result = copyNode(node->left);
//                 }
//                 break;
//
//             case RAIZE:
//                 // 2^3 = 8
//                 if (bothAreNums(node->left, node->right))
//                 {
//                     result = NUM_(pow(node->left->object.constant, node->right->object.constant));
//                 }
//                 // x^1 = x
//                 else if (isOne(node->right))
//                 {
//                     result = copyNode(node->left);
//                 }
//                 // x^0 = 1
//                 else if (isZero(node->right))
//                 {
//                     result = NUM_(1);
//                 }
//                 // 1^x = 1
//                 else if (isOne(node->left))
//                 {
//                     result = NUM_(1);
//                 }
//                 // 0^x = 0
//                 else if (isZero(node->left))
//                 {
//                     result = NUM_(0);
//                 }
//                 break;
//
//             default:
//                 break;
//         }
//     }
//     // если узел изменился то выводим егогг
//     if (result != node && texFile != NULL)
//     {
//         formulaToLatex(originalTree, texFile, title);
//         treeRecursiveDelete(node);
//         treeRecursiveDelete(originalNode);
//         return result;
//     }
//
//     treeRecursiveDelete(originalNode);
//     return node;
// }

node_t* simplifyAdd(node_t* node)
{
    // 2 + 3 = 5
    if (bothAreNums(node->left, node->right))
    {
        return NUM_(node->left->object.constant + node->right->object.constant);
    }
    // 0 + x = x
    else if (isZero(node->left))
    {
        return copyNode(node->right);
    }
    // x + 0 = x
    else if (isZero(node->right))
    {
        return copyNode(node->left);
    }

    return NULL;
}

node_t* simplifySub(node_t* node)
{
    // 5 - 3 = 2
    if (bothAreNums(node->left, node->right))
    {
        return NUM_(node->left->object.constant - node->right->object.constant);
    }
    // x - 0 = x
    else if (isZero(node->right))
    {
        return copyNode(node->left);
    }
    // 0 - x = -x
    else if (isZero(node->left))
    {
        return MUL_(NUM_(-1), copyNode(node->right));
    }

    return NULL;
}

node_t* simplifyMul(node_t* node)
{
    // 2 * 3 = 6
    if (bothAreNums(node->left, node->right))
    {
        return NUM_(node->left->object.constant * node->right->object.constant);
    }
    // 0 * x = 0
    else if (isZero(node->left))
    {
        return NUM_(0);
    }
    // x * 0 = 0
    else if (isZero(node->right))
    {
        return NUM_(0);
    }
    // 1 * x = x
    else if (isOne(node->left))
    {
        return copyNode(node->right);
    }
    // x * 1 = x
    else if (isOne(node->right))
    {
        return copyNode(node->left);
    }

    return NULL;
}

node_t* simplifyDiv(node_t* node)
{
    // 6 / 3 = 2
    if (bothAreNums(node->left, node->right))
    {
        return NUM_(node->left->object.constant / node->right->object.constant);
    }
    // 0 / x = 0
    else if (isZero(node->left))
    {
        return NUM_(0);
    }
    // x / 1 = x
    else if (isOne(node->right))
    {
        return copyNode(node->left);
    }

    return NULL;
}

node_t* simplifyRaize(node_t* node)
{
    // 2^3 = 8
    if (bothAreNums(node->left, node->right))
    {
        return NUM_(pow(node->left->object.constant, node->right->object.constant));
    }
    // x^1 = x
    else if (isOne(node->right))
    {
        return copyNode(node->left);
    }
    // x^0 = 1
    else if (isZero(node->right))
    {
        return NUM_(1);
    }
    // 1^x = 1
    else if (isOne(node->left))
    {
        return NUM_(1);
    }
    // 0^x = 0
    else if (isZero(node->left))
    {
        return NUM_(0);
    }

    return NULL;
}

node_t* simplification(node_t* node, tree_t* originalTree, FILE* texFile)
{
    if (node == NULL)
    {
        return NULL;
    }

    const char* title = "шаг упрощения:";

    node->left = simplification(node->left, originalTree, texFile);
    node->right = simplification(node->right, originalTree, texFile);

    node_t* originalNode = copyNode(node);
    node_t* result = node;  // по умолчанию не меняем

    if (node->type == OP)
    {
        switch (node->object.operation) {
            case ADD:
                result = simplifyAdd(node);
                break;

            case SUB:
                result = simplifySub(node);
                break;

            case MUL:
                result = simplifyMul(node);
                break;

            case DIV:
                result = simplifyDiv(node);
                break;

            case RAIZE:
                result = simplifyRaize(node);
                break;

            default:
                break;
        }
    }

    if (result != node && result != NULL)
    {
        if (texFile != NULL)
        {
            formulaToLatex(originalTree, texFile, title);
        }
        treeRecursiveDelete(node);
        treeRecursiveDelete(originalNode);
        return result;
    }

    treeRecursiveDelete(originalNode);
    return node;
}



bool bothAreNums(node_t* left, node_t* right)
{
    return left != NULL && right != NULL && left->type == NUM && right->type == NUM;
}

bool isOne(node_t* node)
{
    return node != NULL && node->type == NUM && fabs(node->object.constant - 1.0) < 1e-10;
}

bool isZero(node_t* node)
{
    return node != NULL && node->type == NUM && fabs(node->object.constant - 0.0) < 1e-10;          // специально под погрешность - потому что в некоторых случаях подсчет такой
}

#include "DSL_undef.h"

