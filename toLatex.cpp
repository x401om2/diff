#include "diff.h"
#include <stdio.h>

void nodeToLatex(node_t* node, FILE* tex_file)
{
    if (node == NULL) return;

    switch (node->type) {
        case NUM:
            fprintf(tex_file, "%.2f", node->object.constant);
            break;
        case VAR:
            fprintf(tex_file, "%s", node->object.var);
            break;
        case OP:
            switch (node->object.operation) {
                case ADD:
                    fprintf(tex_file, "(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, " + ");
                    nodeToLatex(node->right, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case SUB:
                    fprintf(tex_file, "(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, " - ");
                    nodeToLatex(node->right, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case MUL:
                    fprintf(tex_file, "(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, " \\cdot ");
                    nodeToLatex(node->right, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case DIV:
                    fprintf(tex_file, "\\frac{");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, "}{");
                    nodeToLatex(node->right, tex_file);
                    fprintf(tex_file, "}");
                    break;
                case SIN:
                    fprintf(tex_file, "\\sin(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case COS:
                    fprintf(tex_file, "\\cos(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case TG:
                    fprintf(tex_file, "\\tan(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case CTG:
                    fprintf(tex_file, "\\ctg(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case ARCSIN:
                    fprintf(tex_file, "\\arcsin(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case ARCCOS:
                    fprintf(tex_file, "\\arccos(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case ARCTG:
                    fprintf(tex_file, "\\arctg(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case ARCCTG:
                    fprintf(tex_file, "\\arcctg(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case LN:
                    fprintf(tex_file, "\\ln(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case RAIZE:
                    fprintf(tex_file, "(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")^{");
                    nodeToLatex(node->right, tex_file);
                    fprintf(tex_file, "}");
                    break;
                case SQRT:
                    fprintf(tex_file, "\\sqrt{");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, "}");
                    break;
                case SH:
                    fprintf(tex_file, "\\sinh(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case CH:
                    fprintf(tex_file, "\\cosh(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case TH:
                    fprintf(tex_file, "\\tanh(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case CTH:
                    fprintf(tex_file, "\\coth(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case HZ_OPERATION:
                default:
                    fprintf(tex_file, "?");
                    break;
            }
            break;
    }
}

void formulaToLatex(tree_t* tree, FILE* tex_file, const char* title)
{
    fprintf(tex_file, "\\section*{%s}\n\n", title);
    fprintf(tex_file, "\\[");
    nodeToLatex(tree->root, tex_file);
    fprintf(tex_file, "\\]\n\n");
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


void createFullReport(tree_t* original, tree_t* simplified, tree_t* derivative, VariableTable* table, const char* filename)
{
    FILE* tex_file = fopen(filename, "w");

    fprintf(tex_file, "\\documentclass{article}\n");
    fprintf(tex_file, "\\usepackage{amsmath}\n");
    fprintf(tex_file, "\\usepackage[russian]{babel}\n");
    fprintf(tex_file, "\\begin{document}\n\n");

    fprintf(tex_file, "\\title{Отчет по дифференцированию}\n");
    fprintf(tex_file, "\\maketitle\n\n");


    formulaToLatex(original, tex_file, "Исходное выражение");


    formulaToLatex(simplified, tex_file, "Упрощенное выражение");


    fprintf(tex_file, "\\section*{Значения переменных}\n\n");
    variablesTableToLatex(table, tex_file);


    formulaWithComputationToLatex(simplified, table, tex_file, "Вычисление упрощенного выражения");


    formulaToLatex(derivative, tex_file, "Производная функции");


    formulaWithComputationToLatex(derivative, table, tex_file, "Вычисление производной");


    fprintf(tex_file, "\\section*{Итоговые результаты}\n\n");
    fprintf(tex_file, "\\begin{itemize}\n");

    double func_result = countingTree(simplified->root, table);
    double deriv_result = countingTree(derivative->root, table);

    fprintf(tex_file, "\\item Значение функции: $%.4f$\n", func_result);
    fprintf(tex_file, "\\item Значение производной: $%.4f$\n", deriv_result);
    fprintf(tex_file, "\\end{itemize}\n");

    fprintf(tex_file, "\\end{document}");
    fclose(tex_file);
}
