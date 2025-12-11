#include <stdio.h>
#include "diff.h"
#include "dump.h"
#include "proizv.h"
#include "DSL.h"



tree_t* diffTree(const tree_t* tree)     // var - по какой переменной дифференцируем
{
    tree_t* afterDiffTree = treeCtor();                                         // созд дерево в которое запишем дифф оригинальное дерево

    afterDiffTree->root = diffNode(tree->root);

    return afterDiffTree;
}


node_t* diffNode(node_t* node)
{
    if (node == NULL)
    {
        return NULL;
    }

    switch (node->type) {
        case NUM:
            return NUM_(0);

        case VAR:
            if (strcmp(node->object.var, "x") == 0)
            {
                return NUM_(1);
            }
            else
            {
                return NUM_(0);
            }

        case OP:
            return differenciateOperation(node);
    }

}


node_t* differenciateOperation(node_t* node)
{
    switch (node->object.operation) {
        case ADD:
            {
                return ADD_(DL(node), DR(node));
            }
        case SUB:
            {
                return SUB_(DL(node), DR(node));
            }
        case MUL:
            {
                return ADD_(MUL_(DL(node), CR(node)), MUL_(CL(node), DR(node)));
            }
        case DIV:
            {
                // (u/v)' = (u'v - uv') / v²
                return DIV_(
                    SUB_(
                        MUL_(DL(node), CR(node)),  // u'v
                        MUL_(CL(node), DR(node))   // uv'
                    ),
                    POW_(CR(node), NUM_(2))                     // v²
                );
            }
        case SIN:
            {
                // sin(u)' = cos(u) * u'
                return MUL_(COS_(CL(node)), DL(node));
            }
        case COS:
            {
                // cos(u)' = -sin(u) * u'
                return MUL_(MUL_(NUM_(-1), SIN_(CL(node))), DL(node));
            }
        case TG:
            {
                // tg(u)' = u' / cos²(u)
                return DIV_(DL(node), POW_(COS_(CL(node)), NUM_(2)));
            }
        case RAIZE:
            {
                bool varInLeft = containVariable(node->left);
                bool varInRight = containVariable(node->right);

                if (varInLeft && !varInRight)
                {
                    // u^a' = a * u^(a-1) * u'
                    return MUL_(
                        MUL_(
                            CR(node),                         // a
                            POW_(CL(node), SUB_(CR(node), NUM_(1)))  // u^(a-1)
                        ),
                        DL(node)                  // u'
                    );

                }
                else if (!varInLeft && varInRight)
                {
                    // a^u' = a^u * ln(a) * u'
                    return MUL_(
                        MUL_(
                            POW_(CL(node), CR(node)),          // a^u
                            LN_(CL(node))                      // ln(a)
                        ),
                        DR(node)                  // u'
                    );

                }
                else if (varInLeft && varInRight)
                {
                    // u^v' = u^v * (v'*ln(u) + v*u'/u)
                    return MUL_(
                        POW_(CL(node), CR(node)),               // u^v
                        ADD_(
                            MUL_(DR(node), LN_(CL(node))),   // v'*ln(u)
                            DIV_(MUL_(CR(node), DL(node)), CL(node))  // v*u'/u
                        )
                    );
                }

                return NUM_(0);
            }
        case SQRT:
            {
                // sqrt(u)' = u' / (2*sqrt(u))
                return DIV_(DL(node), MUL_(NUM_(2), SQRT_(CL(node))));
            }

        case SH:
            {
                // sh(u)' = ch(u) * u'
                return MUL_(CH_(CL(node)), DL(node));
            }
        case CH:
            {
                // ch(u)' = sh(u) * u'
                return MUL_(SH_(CL(node)), DL(node));
            }
        case TH:
            {
                // th(u)' = u' / ch²(u)
                return DIV_(DL(node), POW_(CH_(CL(node)), NUM_(2)));
            }
        case CTH:
            {
                // cth(u)' = -u' / sh²(u)
                return DIV_(MUL_(NUM_(-1), DL(node)), POW_(SH_(CL(node)), NUM_(2)));
            }
        default:
            {
                return NUM_(0);
            }
    }
}



node_t* copyNode(node_t* originalNode)
{
    if (originalNode == NULL)
    {
        return NULL;
    }

    node_t* newNode = NULL;                                                     // указ на новый узел пока NULL

    const char* nodeValue = getNodeValueString(originalNode);                   // получ значение ориг узла

    switch (originalNode->type) {                                               // в зависимости от типа знач создаем новый узел
        case OP:
            newNode = createTypedNode(OP, nodeValue, copyNode(originalNode->left), copyNode(originalNode->right));
            break;
        case NUM:
            newNode = NUM_(originalNode->object.constant);
            break;
        case VAR:
            newNode = VAR_(nodeValue);
            break;
    }
    return newNode;
}


bool containVariable(node_t* node)
{
    if (node == NULL)
    {
        return false;
    }

    if (node->type == VAR && strcmp(node->object.var, "x") == 0)                // рекурсивно обходим ноды чтобы понять содержится ли переменная или нет
    {
        return true;
    }
    return containVariable(node->left) || containVariable(node->right);
}


#include "DSL_undef.h"
