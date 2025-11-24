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
                case MUL:
                    fprintf(tex_file, "(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, " \\cdot ");
                    nodeToLatex(node->right, tex_file);
                    fprintf(tex_file, ")");
                    break;
                case SIN:
                    fprintf(tex_file, "\\\\sin(");
                    nodeToLatex(node->left, tex_file);
                    fprintf(tex_file, ")");
                    break;
                // TODO другие операции добавляются по аналогии
            }
            break;
    }
}

void treeToLatexFile(tree_t* tree, VariableTable* table, const char* filename)
{
    FILE* tex_file = fopen(filename, "w");

    fprintf(tex_file, "\\documentclass{article}\n");
    fprintf(tex_file, "\\usepackage{amsmath}\n");
    fprintf(tex_file, "\\begin{document}\n\n");

    fprintf(tex_file, "\\section*{Expression Dump}\n\n");

    fprintf(tex_file, "Expression: ");
    fprintf(tex_file, "\\[");
    nodeToLatex(tree->root, tex_file);
    fprintf(tex_file, "\\]\n\n");

    double result = countingTree(tree->root, table);
    fprintf(tex_file, "Result with substituted values: $%.2f$\n\n", result);

    fprintf(tex_file, "\\begin{tabular}{|c|c|}\n");
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "Variable & Value \\\\\n");
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

    fprintf(tex_file, "\\end{document}");
    fclose(tex_file);
}
