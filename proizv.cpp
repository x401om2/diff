#include <stdio.h>
#include "../INCLUDES/diff.h"
#include "../INCLUDES/dump.h"
#include "../INCLUDES/proizv.h"
#include "../INCLUDES/DSL.h"



tree_t* diffTree(const tree_t* tree, VariableTable* table, const char* var)     // var - по какой переменной дифференцируем
{
    tree_t* afterDiffTree = treeCtor();                                         // созд дерево в которое запишем дифф оригинальное дерево

    afterDiffTree->root = diffNode(tree->root, var, table);

    return afterDiffTree;
}


node_t* diffNode(node_t* node, const char* var, VariableTable* table)
{
    if (node == NULL) return NULL;

    switch (node->type) {
        case NUM:
            return NUM_(0);

        case VAR:
            if (strcmp(node->object.var, var) == 0)
            {
                return NUM_(1);
            } else {
                return NUM_(0);
            }

        case OP:
            return differenciateOperation(node, table, var);
    }

}


node_t* differenciateOperation(node_t* node, VariableTable* table, const char* var)
{
    switch (node->object.operation) {
        case ADD:
            {
                return ADD_(DL(node, var, table), DR(node, var, table));
            }
        case SUB:
            {
                return SUB_(DL(node, var, table), DR(node, var, table));
            }
        case MUL:
            {
                return ADD_(MUL_(DL(node, var, table), CR(node)), MUL_(CL(node), DR(node, var, table)));
            }
        case DIV:
            {
                // (u/v)' = (u'v - uv') / v²
                return DIV_(
                    SUB_(
                        MUL_(DL(node, var, table), CR(node)),  // u'v
                        MUL_(CL(node), DR(node, var, table))   // uv'
                    ),
                    POW_(CR(node), NUM_(2))                     // v²
                );
            }
        case SIN:
            {
                // sin(u)' = cos(u) * u'
                return MUL_(COS_(CL(node)), DL(node, var, table));
            }
        case COS:
            {
                // cos(u)' = -sin(u) * u'
                return MUL_(MUL_(NUM_(-1), SIN_(CL(node))), DL(node, var, table));
            }
        case TG:
            {
                // tg(u)' = u' / cos²(u)
                return DIV_(DL(node, var, table), POW_(COS_(CL(node)), NUM_(2)));
            }
        case CTG:
            {
                // ctg(u)' = -u' / sin²(u)
                return DIV_(
                    MUL_(NUM_(-1), DL(node, var, table)),
                    POW_(SIN_(CL(node)), NUM_(2))
                );
            }
        case ARCSIN:
            {
                // arcsin(u)' = u' / sqrt(1 - u²)
                return DIV_(
                    DL(node, var, table),
                    SQRT_(SUB_(NUM_(1), POW_(CL(node), NUM_(2))))
                );
            }
        case ARCCOS:
            {
                // arccos(u)' = -u' / sqrt(1 - u²)
                return DIV_(
                    MUL_(NUM_(-1), DL(node, var, table)),
                    SQRT_(SUB_(NUM_(1), POW_(CL(node), NUM_(2))))
                );
            }
        case ARCTG:
            {
                // arctg(u)' = u' / (1 + u²)
                return DIV_(
                    DL(node, var, table),
                    ADD_(NUM_(1), POW_(CL(node), NUM_(2)))
                );
            }
        case ARCCTG:
            {
                // arcctg(u)' = -u' / (1 + u²)
                return DIV_(
                    MUL_(NUM_(-1), DL(node, var, table)),
                    ADD_(NUM_(1), POW_(CL(node), NUM_(2)))
                );
            }
        case LN:
            {
                // ln(u)' = u' / u
                return DIV_(DL(node, var, table), CL(node));
            }
        case RAIZE:
            {
                bool varInLeft = containVariable(node->left, var);
                bool varInRight = containVariable(node->right, var);

                if (varInLeft && !varInRight)
                {
                    // u^a' = a * u^(a-1) * u'
                    return MUL_(
                        MUL_(
                            CR(node),                         // a
                            POW_(CL(node), SUB_(CR(node), NUM_(1)))  // u^(a-1)
                        ),
                        DL(node, var, table)                  // u'
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
                        DR(node, var, table)                  // u'
                    );

                }
                else if (varInLeft && varInRight)
                {
                    // u^v' = u^v * (v'*ln(u) + v*u'/u)
                    return MUL_(
                        POW_(CL(node), CR(node)),               // u^v
                        ADD_(
                            MUL_(DR(node, var, table), LN_(CL(node))),   // v'*ln(u)
                            DIV_(MUL_(CR(node), DL(node, var, table)), CL(node))  // v*u'/u
                        )
                    );
                }

                return NUM_(0);
            }
        case SQRT:
            {
                // sqrt(u)' = u' / (2*sqrt(u))
                return DIV_(DL(node, var, table), MUL_(NUM_(2), SQRT_(CL(node))));
            }

        case SH:
            {
                // sh(u)' = ch(u) * u'
                return MUL_(CH_(CL(node)), DL(node, var, table));
            }
        case CH:
            {
                // ch(u)' = sh(u) * u'
                return MUL_(SH_(CL(node)), DL(node, var, table));
            }
        case TH:
            {
                // th(u)' = u' / ch²(u)
                return DIV_(DL(node, var, table), POW_(CH_(CL(node)), NUM_(2)));
            }
        case CTH:
            {
                // cth(u)' = -u' / sh²(u)
                return DIV_(MUL_(NUM_(-1), DL(node, var, table)), POW_(SH_(CL(node)), NUM_(2)));
            }
        default:
            {
                return NUM_(0);
            }
    }
}



node_t* copyNode(node_t* originalNode)
{
    if (originalNode == NULL) return NULL;

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


bool containVariable(node_t* node, const char* var)
{
    if (node == NULL) return false;

    if (node->type == VAR && strcmp(node->object.var, var) == 0)                // рекурсивно обходим ноды чтобы понять содержится ли переменная или нет
    {
        return true;
    }
    return containVariable(node->left, var) || containVariable(node->right, var);
}
