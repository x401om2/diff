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
    if (node == NULL) return NULL;

    switch (node->type) {
        case NUM:
            return createNumNode(0);

        case VAR:
            if (strcmp(node->object.var, var) == 0)
            {
                return createNumNode(1);
            } else {
                return createNumNode(0);
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
        case DIV:
            {
                // (u/v)' = (u'v - uv') / v²
                node_t* u = node->left;
                node_t* v = node->right;
                node_t* u_prime = diffNode(u, var, table);
                node_t* v_prime = diffNode(v, var, table);

                node_t* numerator_left = createTypedNode(OP, "*", u_prime, copyNode(v));
                node_t* numerator_right = createTypedNode(OP, "*", copyNode(u), v_prime);
                node_t* numerator = createTypedNode(OP, "-", numerator_left, numerator_right);

                node_t* denominator = createTypedNode(OP, "^", copyNode(v), createNumNode(2));

                node_t* result = createTypedNode(OP, "/", numerator, denominator);
                return result;
            }
        case SIN:
            {
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* cosNode = createTypedNode(OP, "cos", copyNode(arg), NULL);
                node_t* result = createTypedNode(OP, "*", cosNode, diff_arg);
                return result;
            }
        case COS:
            {
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* minus_one = createNumNode(-1);
                node_t* sinNode = createTypedNode(OP, "sin", copyNode(arg), NULL);
                node_t* minus_sin = createTypedNode(OP, "*", minus_one, sinNode);
                node_t* result = createTypedNode(OP, "*", minus_sin, diff_arg);
                return result;
            }
        case TG:
            {
                // tg(u)' = u' / cos²(u)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* cosNode = createTypedNode(OP, "cos", copyNode(arg), NULL);
                node_t* cosSquared = createTypedNode(OP, "^", cosNode, createNumNode(2));
                node_t* result = createTypedNode(OP, "/", diff_arg, cosSquared);
                return result;
            }
        case CTG:
            {
                // ctg(u)' = -u' / sin²(u)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* minus_one = createNumNode(-1);
                node_t* sinNode = createTypedNode(OP, "sin", copyNode(arg), NULL);
                node_t* sinSquared = createTypedNode(OP, "^", sinNode, createNumNode(2));
                node_t* numerator = createTypedNode(OP, "*", minus_one, diff_arg);
                node_t* result = createTypedNode(OP, "/", numerator, sinSquared);
                return result;
            }
        case ARCSIN:
            {
                // arcsin(u)' = u' / sqrt(1 - u²)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* one = createNumNode(1);
                node_t* uSquared = createTypedNode(OP, "^", copyNode(arg), createNumNode(2));
                node_t* one_minus_u2 = createTypedNode(OP, "-", one, uSquared);
                node_t* sqrtNode = createTypedNode(OP, "sqrt", one_minus_u2, NULL);
                node_t* result = createTypedNode(OP, "/", diff_arg, sqrtNode);
                return result;
            }
        case ARCCOS:
            {
                // arccos(u)' = -u' / sqrt(1 - u²)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* minus_one = createNumNode(-1);
                node_t* one = createNumNode(1);
                node_t* uSquared = createTypedNode(OP, "^", copyNode(arg), createNumNode(2));
                node_t* one_minus_u2 = createTypedNode(OP, "-", one, uSquared);
                node_t* sqrtNode = createTypedNode(OP, "sqrt", one_minus_u2, NULL);
                node_t* numerator = createTypedNode(OP, "*", minus_one, diff_arg);
                node_t* result = createTypedNode(OP, "/", numerator, sqrtNode);
                return result;
            }
        case ARCTG:
            {
                // arctg(u)' = u' / (1 + u²)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* one = createNumNode(1);
                node_t* uSquared = createTypedNode(OP, "^", copyNode(arg), createNumNode(2));
                node_t* denominator = createTypedNode(OP, "+", one, uSquared);
                node_t* result = createTypedNode(OP, "/", diff_arg, denominator);
                return result;
            }
        case ARCCTG:
            {
                // arcctg(u)' = -u' / (1 + u²)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* minus_one = createNumNode(-1);
                node_t* one = createNumNode(1);
                node_t* uSquared = createTypedNode(OP, "^", copyNode(arg), createNumNode(2));
                node_t* denominator = createTypedNode(OP, "+", one, uSquared);
                node_t* numerator = createTypedNode(OP, "*", minus_one, diff_arg);
                node_t* result = createTypedNode(OP, "/", numerator, denominator);
                return result;
            }
        case LN:
            {
                // ln(u)' = u' / u
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* result = createTypedNode(OP, "/", diff_arg, copyNode(arg));
                return result;
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
                    node_t* aMinusOne = createTypedNode(OP, "-", copyNode(node->right), createNumNode(1));
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
                    node_t* firstMul = createTypedNode(OP, "*", copyNode(node), ln_a);
                    node_t* result = createTypedNode(OP, "*", firstMul, pow_u);
                    return result;

                } else if (varInLeft && varInRight) {
                    // случай когда х и в степени и в основании osn^step
                    node_t* osn_node = copyNode(node->left);
                    node_t* step_node = copyNode(node->right);
                    node_t* pow = createTypedNode(OP, "^", osn_node, step_node);
                    node_t* diff_step = diffNode(step_node, var, table);
                    node_t* ln_osn = createTypedNode(OP, "ln", osn_node, NULL);
                    node_t* firstMul = createTypedNode(OP, "*", diff_step, ln_osn);
                    node_t* diff_osn = diffNode(osn_node, var, table);
                    node_t* secondMul = createTypedNode(OP, "*", step_node, diff_osn);
                    node_t* firstDiv = createTypedNode(OP, "/", secondMul, osn_node);
                    node_t* firstAdd = createTypedNode(OP, "+", firstMul, firstDiv);
                    node_t* result = createTypedNode(OP, "*", pow, firstAdd);
                    return result;
                }
            }
        case SQRT:
            {
                // sqrt(u)' = u' / (2 * sqrt(u))
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* two = createNumNode(2);
                node_t* sqrt_u = createTypedNode(OP, "sqrt", copyNode(arg), NULL);
                node_t* denominator = createTypedNode(OP, "*", two, sqrt_u);
                node_t* result = createTypedNode(OP, "/", diff_arg, denominator);
                return result;
            }

        case SH:
            {
                // sh(u)' = ch(u) * u'
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* chNode = createTypedNode(OP, "ch", copyNode(arg), NULL);
                node_t* result = createTypedNode(OP, "*", chNode, diff_arg);
                return result;
            }
        case CH:
            {
                // ch(u)' = sh(u) * u'
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* shNode = createTypedNode(OP, "sh", copyNode(arg), NULL);
                node_t* result = createTypedNode(OP, "*", shNode, diff_arg);
                return result;
            }
        case TH:
            {
                // th(u)' = u' / ch²(u)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* chNode = createTypedNode(OP, "ch", copyNode(arg), NULL);
                node_t* chSquared = createTypedNode(OP, "^", chNode, createNumNode(2));
                node_t* result = createTypedNode(OP, "/", diff_arg, chSquared);
                return result;
            }
        case CTH:
            {
                // cth(u)' = -u' / sh²(u)
                node_t* arg = node->left;
                node_t* diff_arg = diffNode(arg, var, table);
                node_t* minus_one = createNumNode(-1);
                node_t* shNode = createTypedNode(OP, "sh", copyNode(arg), NULL);
                node_t* shSquared = createTypedNode(OP, "^", shNode, createNumNode(2));
                node_t* numerator = createTypedNode(OP, "*", minus_one, diff_arg);
                node_t* result = createTypedNode(OP, "/", numerator, shSquared);
                return result;
            }
        default:
            {
                return createNumNode(0);
            }
    }
}



node_t* copyNode(node_t* originalNode)
{
    if (originalNode == NULL) return NULL;

    node_t* newNode = NULL;

    const char* nodeValue = getNodeValueString(originalNode);

    switch (originalNode->type) {
        case OP:
            newNode = createTypedNode(OP, nodeValue, copyNode(originalNode->left), copyNode(originalNode->right));
            break;
        case NUM:
            newNode = createNumNode(originalNode->object.constant);
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
