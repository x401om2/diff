#include "../INCLUDES/diff.h"
#include "../INCLUDES/proizv.h"
#include "../INCLUDES/simply.h"
#include <stdio.h>
#include <stdbool.h>


bool needsParenthesesForOp(node_t* node, int parentPriority)
{
    if (node == NULL || node->type != OP) return false;

    int currentPriority = 0;

    switch (node->object.operation) {
        case ADD:
        case SUB:
            currentPriority = 1;
            break;
        case MUL:
        case DIV:
            currentPriority = 2;
            break;
        case RAIZE:
            currentPriority = 3;
            break;
        case SIN:
        case COS:
        case TG:
        case CTG:
        case ARCSIN:
        case ARCCOS:
        case ARCTG:
        case ARCCTG:
        case LN:
        case SQRT:
        case SH:
        case CH:
        case TH:
        case CTH:
            currentPriority = 4;
            break;
        default:
            return false;
    }
    return currentPriority < parentPriority;
}

void nodeToLatexWithPriority(node_t* node, FILE* tex_file, int parentPriority)
{
    if (node == NULL) return;

    switch (node->type) {
        case NUM:
            fprintf(tex_file, "%g", node->object.constant);
            break;
        case VAR:
            fprintf(tex_file, "%s", node->object.var);
            break;
        case OP:
            bool needsParens = needsParenthesesForOp(node, parentPriority);

            if (needsParens) fprintf(tex_file, "\\left(");

            switch (node->object.operation) {
                case ADD:
                    nodeToLatexWithPriority(node->left, tex_file, 1);
                    fprintf(tex_file, " + ");
                    nodeToLatexWithPriority(node->right, tex_file, 1);
                    break;

                case SUB:
                    nodeToLatexWithPriority(node->left, tex_file, 1);
                    fprintf(tex_file, " - ");
                    nodeToLatexWithPriority(node->right, tex_file, 1);
                    break;

                case MUL:
                    nodeToLatexWithPriority(node->left, tex_file, 2);
                    fprintf(tex_file, " \\cdot ");
                    nodeToLatexWithPriority(node->right, tex_file, 2);
                    break;

                case DIV:
                    fprintf(tex_file, "\\frac{");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "}{");
                    nodeToLatexWithPriority(node->right, tex_file, 0);
                    fprintf(tex_file, "}");
                    break;

                case SIN:
                    fprintf(tex_file, "\\sin\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case COS:
                    fprintf(tex_file, "\\cos\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case TG:
                    fprintf(tex_file, "\\tan\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case CTG:
                    fprintf(tex_file, "\\ctg\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case ARCSIN:
                    fprintf(tex_file, "\\arcsin\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case ARCCOS:
                    fprintf(tex_file, "\\arccos\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case ARCTG:
                    fprintf(tex_file, "\\arctg\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case ARCCTG:
                    fprintf(tex_file, "\\arcctg\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case LN:
                    fprintf(tex_file, "\\ln\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case RAIZE:
                    if (node->left && (node->left->type == VAR || (node->left->type == NUM && node->left->object.constant >= 0)))
                    {
                        nodeToLatexWithPriority(node->left, tex_file, 3);
                    } else {
                        fprintf(tex_file, "\\left(");
                        nodeToLatexWithPriority(node->left, tex_file, 0);
                        fprintf(tex_file, "\\right)");
                    }
                    fprintf(tex_file, "^{");
                    nodeToLatexWithPriority(node->right, tex_file, 0);
                    fprintf(tex_file, "}");
                    break;

                case SQRT:
                    fprintf(tex_file, "\\sqrt{");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "}");
                    break;

                case SH:
                    fprintf(tex_file, "\\sinh\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case CH:
                    fprintf(tex_file, "\\cosh\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case TH:
                    fprintf(tex_file, "\\tanh\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case CTH:
                    fprintf(tex_file, "\\coth\\left(");
                    nodeToLatexWithPriority(node->left, tex_file, 0);
                    fprintf(tex_file, "\\right)");
                    break;

                case HZ_OPERATION:
                default:
                    fprintf(tex_file, "?");
                    break;
            }

            if (needsParens) fprintf(tex_file, "\\right)");
            break;
    }
}


void nodeToLatex(node_t* node, FILE* tex_file)
{
    nodeToLatexWithPriority(node, tex_file, 0);
}


void formulaToLatex(tree_t* tree, FILE* tex_file, const char* title)
{
    if (title && title[0] != '\0') {
        fprintf(tex_file, "\\subsection*{%s}\n", title);
    }
    fprintf(tex_file, "\\begin{dmath}\n");
    nodeToLatex(tree->root, tex_file);
    fprintf(tex_file, "\n\\end{dmath}\n\n");
}

void formulaWithComputationToLatex(tree_t* tree, VariableTable* table, FILE* tex_file, const char* title)
{
    formulaToLatex(tree, tex_file, title);

    double result = countingTree(tree->root, table);
    fprintf(tex_file, "Результат: \\[%.4f\\]\n\n", result);
}

void variablesTableToLatex(VariableTable* table, FILE* tex_file)
{
    fprintf(tex_file, "\\begin{tabular}{|c|c|}\n");
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "Переменная & Значение \\\\\n");
    fprintf(tex_file, "\\hline\n");
    for (int i = 0; i < table->count; i++)
    {
        if (table->variables[i].isDefined)
        {
            fprintf(tex_file, "$%s$ & $%.2f$ \\\\\n", table->variables[i].name, table->variables[i].value);
        }
    }
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "\\end{tabular}\n\n");
}


void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename)
{
    FILE* tex_file = fopen(filename, "w");

    fprintf(tex_file, "\\documentclass{article}\n");
    fprintf(tex_file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(tex_file, "\\usepackage{amsmath}\n");
    fprintf(tex_file, "\\usepackage{breqn}\n");
    fprintf(tex_file, "\\usepackage[russian]{babel}\n");
    fprintf(tex_file, "\\usepackage{hyperref}\n");
    fprintf(tex_file, "\\usepackage{geometry}\n");
    fprintf(tex_file, "\\geometry{a4paper, left=20mm, right=20mm, top=20mm, bottom=20mm}\n");
    fprintf(tex_file, "\\setlength{\\parindent}{0pt}\n");
    fprintf(tex_file, "\\setlength{\\parskip}{1em}\n");

    fprintf(tex_file, "\\title{Математический отчет}\n");
    fprintf(tex_file, "\\author{ЯРИК}\n");
    fprintf(tex_file, "\\date{\\today}\n");
    fprintf(tex_file, "\\begin{document}\n");
    fprintf(tex_file, "\\maketitle\n");
    fprintf(tex_file, "\\tableofcontents\n");
    fprintf(tex_file, "\\newpage\n");

    fprintf(tex_file, "\\section{Исходные данные}\n\n");
    fprintf(tex_file, "\\subsection{Исходное выражение}\n");
    fprintf(tex_file, "\\begin{dmath}\n");
    nodeToLatex(original->root, tex_file);
    fprintf(tex_file, "\n\\end{dmath}\n\n");

    fprintf(tex_file, "\\subsection{Таблица переменных}\n\n");
    variablesTableToLatex(table, tex_file);

    double original_result = countingTree(original->root, table);
    fprintf(tex_file, "Значение исходного выражения: \\[%.4f\\]\n\n", original_result);

    fprintf(tex_file, "\\section{Упрощение выражения}\n\n");

    tree_t* simplified = treeCtor();
    simplified->root = copyNode(original->root);

    fprintf(tex_file, "\\subsection{Поэтапное упрощение}\n\n");
    simplified->root = simplification(simplified->root, simplified, tex_file);

    fprintf(tex_file, "\\subsection{Финальное упрощенное выражение}\n");
    fprintf(tex_file, "\\begin{dmath}\n");
    nodeToLatex(simplified->root, tex_file);
    fprintf(tex_file, "\n\\end{dmath}\n\n");

    double simplified_result = countingTree(simplified->root, table);
    fprintf(tex_file, "Значение упрощенного выражения: \\[%.4f\\]\n\n", simplified_result);

    fprintf(tex_file, "\\section{Производная по переменной $x$}\n\n");

    tree_t* derivative = diffTree(simplified, table, "x");

    if (derivative != NULL && derivative->root != NULL)
    {
        fprintf(tex_file, "\\subsection{Исходная производная}\n");
        fprintf(tex_file, "\\begin{dmath}\n");
        nodeToLatex(derivative->root, tex_file);
        fprintf(tex_file, "\n\\end{dmath}\n\n");

        fprintf(tex_file, "\\subsection{Упрощенная производная}\n");
        derivative->root = simplification(derivative->root, derivative, tex_file);
        fprintf(tex_file, "\\begin{dmath}\n");
        nodeToLatex(derivative->root, tex_file);
        fprintf(tex_file, "\n\\end{dmath}\n\n");

        double deriv_result = countingTree(derivative->root, table);
        fprintf(tex_file, "Значение производной: \\[%.4f\\]\n\n", deriv_result);

        fprintf(tex_file, "\\section{Финальные результаты}\n\n");

        fprintf(tex_file, "\\subsection{Упрощенное выражение и его производная}\n");

        fprintf(tex_file, "\\begin{align*}\n");
        fprintf(tex_file, "f(x) &= ");
        nodeToLatex(simplified->root, tex_file);
        fprintf(tex_file, " \\\\\n");
        fprintf(tex_file, "f'(x) &= ");
        nodeToLatex(derivative->root, tex_file);
        fprintf(tex_file, "\n\\end{align*}\n\n");

        fprintf(tex_file, "\\begin{itemize}\n");
        fprintf(tex_file, "\\item Значение упрощенного выражения: $f(%.2f) = %.4f$\n",
                (table->count > 0 && table->variables[0].isDefined) ? table->variables[0].value : 0.0,
                simplified_result);
        fprintf(tex_file, "\\item Значение производной: $f'(%.2f) = %.4f$\n",
                (table->count > 0 && table->variables[0].isDefined) ? table->variables[0].value : 0.0,
                deriv_result);
        fprintf(tex_file, "\\end{itemize}\n\n");

        treeRecursiveDelete(derivative->root);
        free(derivative);
    }

    fprintf(tex_file, "\\section{Итоговая таблица}\n\n");
    fprintf(tex_file, "\\begin{tabular}{|l|c|}\n");
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "Параметр & Значение \\\\\n");
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "Исходное выражение & $%.4f$ \\\\\n", original_result);
    fprintf(tex_file, "Упрощенное выражение & $%.4f$ \\\\\n", simplified_result);

    tree_t* deriv_for_summary = diffTree(simplified, table, "x");
    if (deriv_for_summary != NULL && deriv_for_summary->root != NULL)
    {
        deriv_for_summary->root = simplification(deriv_for_summary->root, deriv_for_summary, NULL);
        double deriv_val = countingTree(deriv_for_summary->root, table);
        fprintf(tex_file, "Производная по $x$ & $%.4f$ \\\\\n", deriv_val);
        treeRecursiveDelete(deriv_for_summary->root);
        free(deriv_for_summary);
    }

    fprintf(tex_file, "Размер исходного дерева & %d узлов \\\\\n", countTreeSize(original->root));
    fprintf(tex_file, "Размер упрощенного дерева & %d узлов \\\\\n", countTreeSize(simplified->root));
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "\\end{tabular}\n");

    treeRecursiveDelete(simplified->root);
    free(simplified);

    fprintf(tex_file, "\\end{document}\n");
    fclose(tex_file);
}
