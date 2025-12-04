#ifndef DSL_H
#define DSL_H

#include "diff.h"

#define CL(node) copyNode((node)->left)
#define CR(node) copyNode((node)->right)

#define DL(node, var, table) diffNode(CL(node), (var), (table))
#define DR(node, var, table) diffNode(CR(node), (var), (table))

#define NUM_(val) createNumNode(val)
#define VAR_(name) createTypedNode(VAR, (name), NULL, NULL)

// операции с двумя параметрами
#define ADD_(left, right) createTypedNode(OP, "+", (left), (right))
#define SUB_(left, right) createTypedNode(OP, "-", (left), (right))
#define MUL_(left, right) createTypedNode(OP, "*", (left), (right))
#define DIV_(left, right) createTypedNode(OP, "/", (left), (right))
#define POW_(left, right) createTypedNode(OP, "^", (left), (right))


// операции с одним параметром
#define SIN_(arg) createTypedNode(OP, "sin", (arg), NULL)
#define COS_(arg) createTypedNode(OP, "cos", (arg), NULL)
#define TG_(arg) createTypedNode(OP, "tg", (arg), NULL)
#define CTG_(arg) createTypedNode(OP, "ctg", (arg), NULL)
#define SH_(arg) createTypedNode(OP, "sh", (arg), NULL)
#define CH_(arg) createTypedNode(OP, "ch", (arg), NULL)
#define TH_(arg) createTypedNode(OP, "th", (arg), NULL)
#define CTH_(arg) createTypedNode(OP, "cth", (arg), NULL)
#define LN_(arg) createTypedNode(OP, "ln", (arg), NULL)
#define SQRT_(arg) createTypedNode(OP, "sqrt", (arg), NULL)

// обратные тригонометрические функции
#define ARCSIN_(arg) createTypedNode(OP, "arcsin", (arg), NULL)
#define ARCCOS_(arg) createTypedNode(OP, "arccos", (arg), NULL)
#define ARCTG_(arg) createTypedNode(OP, "arctg", (arg), NULL)
#define ARCCTG_(arg) createTypedNode(OP, "arcctg", (arg), NULL)

// // проверяем типы узлов
// #define IS_NUM(node) ((node) && (node)->type == NUM)
// #define IS_VAR(node) ((node) && (node)->type == VAR)
// #define IS_OP(node) ((node) && (node)->type == OP)



#endif
