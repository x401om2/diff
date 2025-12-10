#include "diff.h"
#include "proizv.h"
#include "simply.h"
#include "toLatex.h"
#include "graphics.h"
#include <stdio.h>
#include <stdbool.h>


//frac{}{} - дроби
// ^{} - создает верхний индекс степень
// \\begin{dmath}\n - формулы с автоматическими переносами

bool needsParenthesesForOp(node_t* node, int parentPriority)                // нужно ли ставить скобки вокруг оператора при выводе в латех
{
    if (node == NULL || node->type != OP)                                   // узел сущ и явл оператором
    {
        return false;
    }

    int currentPriority = 0;

    switch (node->object.operation) {
        case ADD:
        case SUB:
            currentPriority = 1;                // низкий приоритет
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
            currentPriority = 4;                // самый высокий приоритет
            break;
        default:
            return false;
    }
    return currentPriority < parentPriority;    // скобки нужны - если приоритет текущей операции ниже приоритета родительской операции
}

void nodeToLatexWithPriority(node_t* node, FILE* tex_file, int parentPriority)      // дерево -> laTex с уч приоритетов
{
    if (node == NULL)
    {
        return;
    }

    switch (node->type) {
        case NUM:
            fprintf(tex_file, "%g", node->object.constant);                         // просто печатаем числа и переменные напрямую
            break;
        case VAR:
            fprintf(tex_file, "%s", node->object.var);
            break;
        case OP:

            bool needsParens = needsParenthesesForOp(node, parentPriority);

            if (needsParens)
            {
                fprintf(tex_file, "\\left(");
            }

            switch (node->object.operation) {                                       // рекурсивно все делаем как обычно
                case ADD:
                    nodeToLatexWithPriority(node->left, tex_file, 1);               // для +  выводим рекурсивно и левую и правую часть с приоритетом 1
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
                    }
                    else
                    {
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

            if (needsParens) fprintf(tex_file, "\\right)");             // если была открывающая то нужна и закрывающая
            break;
    }
}


void nodeToLatex(node_t* node, FILE* tex_file)
{
    nodeToLatexWithPriority(node, tex_file, 0);                     // начало с приоритетом 0
}


void formulaToLatex(tree_t* tree, FILE* tex_file, const char* title)    // просто формула
{
    if (title && title[0] != '\0')
    {
        fprintf(tex_file, "\\subsection*{%s}\n", title);
    }
    fprintf(tex_file, "\\begin{dmath}\n");
    nodeToLatex(tree->root, tex_file);
    fprintf(tex_file, "\n\\end{dmath}\n\n");
}


void formulaWithComputationToLatex(tree_t* tree, VariableTable* table, FILE* tex_file, const char* title)   // формула и численное значение
{
    formulaToLatex(tree, tex_file, title);

    double result = countingTree(tree->root, table);
    fprintf(tex_file, "Результат: \\[%.4f\\]\n\n", result);
}


void variablesTableToLatex(VariableTable* table, FILE* tex_file)
{
    fprintf(tex_file,
        "\\begin{tabular}{|c|c|}\n"                 // таблица с двумя колонками и вертик линиями
        "\\hline\n"
        "Переменная & Значение \\\\\n"
        "\\hline\n");

    for (int i = 0; i < table->count; i++)
    {
        if (table->variables[i].isDefined)                          // если определена переменная и имеет значение
        {
            fprintf(tex_file, "$%s$ & $%.2f$ \\\\\n", table->variables[i].name, table->variables[i].value);
        }
    }
    fprintf(tex_file,
        "\\hline\n"
        "\\end{tabular}\n\n");
}



void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename,  float plotMinX, float plotMaxX)
{
    FILE* tex_file = fopen(filename, "w");

    if (!tex_file)
    {
        printf("не сделали файл %s\n", filename);
        return;
    }

    tree_t* simplified = NULL;
    tree_t* derivative = NULL;

    double original_value = 0;
    double simplified_value = 0;
    double derivative_value = 0;

    writeLaTeXPreamble(tex_file);                       // заголовочек
    writeTitleAndTOC(tex_file);

    original_value = writeOriginalDataSection(original, table, tex_file);   // исходные данные

    simplified = writeSimplificationSection(original, table, tex_file, &simplified_value, plotMinX , plotMaxX);  // упрощаем исходное

    if (simplified != NULL && simplified->root != NULL)         // если упростили - берем производную
    {
        derivative = writeDerivativeSection(simplified, table, tex_file, &derivative_value, plotMinX, plotMaxX);
    }

    if (simplified != NULL && derivative != NULL)                // полученные результаты
    {
        writeFinalResultsSection(simplified, derivative, table, tex_file, simplified_value, derivative_value);
    }

    writeSummaryTable(original, simplified, derivative, table, tex_file, original_value, simplified_value, derivative_value);       // итоговая таблица значений

    writeLaTeXFooter(tex_file);                             // закрываем документ
    fclose(tex_file);

    cleanupTrees(simplified, derivative);
}


void writeLaTeXPreamble(FILE* tex_file)
{
    fprintf(tex_file,
        "\\documentclass{article}\n"
        "\\usepackage[utf8]{inputenc}\n"
        "\\usepackage{amsmath}\n"
        "\\usepackage{breqn}\n"
        "\\usepackage[russian]{babel}\n"
        "\\usepackage{geometry}\n"
        "\\usepackage{pgfplots}\n"
        "\\pgfplotsset{compat=1.18}\n"
        "\\geometry{a4paper, left=20mm, right=20mm, top=20mm, bottom=20mm}\n"     // поля со всех сторон по 20мм
        "\\setlength{\\parindent}{0pt}\n"                                         // без отступа у -красной- строки
        "\\setlength{\\parskip}{1em}\n");                                           // отступ между абзацами
}

void writeTitleAndTOC(FILE* tex_file)
{
    fprintf(tex_file,
        "\\title{Математический отчет}\n"
        "\\author{ЯРИК}\n"
        "\\date{\\today}\n"
        "\\begin{document}\n"
        "\\maketitle\n"                        // генерит титульную страницу
        "\\tableofcontents\n"
        "\\newpage\n");                           // нов страница
}

double writeOriginalDataSection(tree_t* original, VariableTable* table, FILE* tex_file)
{
    fprintf(tex_file,
        "\\section{Исходные данные}\n\n"                                        // section раздел автоматически нумеруется
        "\\subsection{Исходное выражение}\n"
        "\\begin{dmath}\n");
    nodeToLatex(original->root, tex_file);

    fprintf(tex_file,
        "\n\\end{dmath}\n\n"
        "\\subsection{Таблица переменных}\n\n");

    variablesTableToLatex(table, tex_file);

    double original_result = countingTree(original->root, table);

    fprintf(tex_file, "Значение исходного выражения: \\[%.4f\\]\n\n", original_result);

    return original_result;
}

tree_t* writeSimplificationSection(tree_t* original, VariableTable* table, FILE* tex_file, double* simplified_value, float plotMinX, float plotMaxX)
{
    fprintf(tex_file, "\\section{Упрощение выражения}\n\n");

    tree_t* simplified = treeCtor();
    if (!simplified)
    {
        fprintf(tex_file, "\\textbf{не получилось сделать дерево для упрощения}\n\n");
        return NULL;
    }

    simplified->root = copyNode(original->root);
    if (!simplified->root)
    {
        fprintf(tex_file, "\\textbf{не получилось скопировать дерево}\n\n");
        free(simplified);
        return NULL;
    }

    fprintf(tex_file, "\\subsection{Поэтапное упрощение}\n\n");
    simplified->root = simplification(simplified->root, simplified, tex_file);

    fprintf(tex_file,
        "\\subsection{Финальное упрощенное выражение}\n"
        "\\begin{dmath}\n");

    nodeToLatex(simplified->root, tex_file);
    fprintf(tex_file, "\n\\end{dmath}\n\n");

    *simplified_value = countingTree(simplified->root, table);

    fprintf(tex_file,
        "Значение упрощенного выражения: \\[%.4f\\]\n\n"
        "\\subsection{График упрощенной функции}\n",
        *simplified_value);

    float step = (plotMaxX - plotMinX) / 100.0;

    addPlotToLatexDirect(simplified, table, tex_file, plotMinX, plotMaxX, step);                      // снова графичек - теперь упрощенной функции

    return simplified;
}

tree_t* writeDerivativeSection(tree_t* simplified, VariableTable* table, FILE* tex_file, double* derivative_value, float plotMinX, float plotMaxX)
{
    fprintf(tex_file, "\\section{Производная по переменной $x$}\n\n");

    tree_t* derivative = diffTree(simplified, table, "x");

    if (derivative == NULL || derivative->root == NULL)
    {
        fprintf(tex_file, "\\textbf{не получилось вычислить производную}\n\n");
        return NULL;
    }

    fprintf(tex_file,
        "\\subsection{Исходная производная}\n"
        "\\begin{dmath}\n");

    nodeToLatex(derivative->root, tex_file);

    fprintf(tex_file,
        "\n\\end{dmath}\n\n"
        "\\subsection{Упрощенная производная}\n");

    derivative->root = simplification(derivative->root, derivative, tex_file);
    fprintf(tex_file, "\\begin{dmath}\n");
    nodeToLatex(derivative->root, tex_file);
    fprintf(tex_file, "\n\\end{dmath}\n\n");

    *derivative_value = countingTree(derivative->root, table);
    fprintf(tex_file,
        "Значение производной: \\[%.4f\\]\n\n"
        "\\subsection{График производной}\n",
        *derivative_value);

    float step = (plotMaxX - plotMinX) / 100.0;
    addPlotToLatexDirect(derivative, table, tex_file, plotMinX, plotMaxX, step);                       // графичек производной

    return derivative;
}

void writeFinalResultsSection(tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* tex_file, double simplified_value, double derivative_value)
{
    fprintf(tex_file,
        "\\section{Финальные результаты}\n\n"
        "\\subsection{Упрощенное выражение и его производная}\n"
        "\\begin{align*}\n"
        "f(x) &= ");

    nodeToLatex(simplified->root, tex_file);

    fprintf(tex_file,
        " \\\\\n"
        "\\frac{d}{dx} &= ");

    nodeToLatex(derivative->root, tex_file);
    fprintf(tex_file, "\n\\end{align*}\n\n");

    double x_value = 0.0;
    if (table->count > 0 && table->variables[0].isDefined)
    {
        x_value = table->variables[0].value;
    }

    fprintf(tex_file,
        "\\begin{itemize}\n"
        "\\item Значение упрощенного выражения: $f(%.2f) = %.4f$\n"
        "\\item Значение производной: $f'(%.2f) = %.4f$\n"
        "\\end{itemize}\n\n",
        x_value, simplified_value, x_value, derivative_value);
}

void writeSummaryTable(tree_t* original, tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* tex_file, double original_value, double simplified_value, double derivative_value)
{
    fprintf(tex_file,
        "\\section{Итоговая таблица}\n\n"
        "\\begin{tabular}{|l|c|}\n"
        "\\hline\n"
        "Параметр & Значение \\\\\n"
        "\\hline\n"
        "Исходное выражение & $%.4f$ \\\\\n"
        "Упрощенное выражение & $%.4f$ \\\\\n",
        original_value, simplified_value);

    if (derivative != NULL && derivative->root != NULL)
    {
        fprintf(tex_file, "Производная по $x$ & $%.4f$ \\\\\n", derivative_value);
    }
    else
    {
        fprintf(tex_file, "Производная по $x$ & не вычислена \\\\\n");
    }

    fprintf(tex_file, "Размер исходного дерева & %d узлов \\\\\n", countTreeSize(original->root));

    if (simplified != NULL && simplified->root != NULL)
    {
        fprintf(tex_file, "Размер упрощенного дерева & %d узлов \\\\\n", countTreeSize(simplified->root));
    }
    else
    {
        fprintf(tex_file, "Размер упрощенного дерева & не определен \\\\\n");
    }

    fprintf(tex_file,
        "\\hline\n"
        "\\end{tabular}\n");
}

void cleanupTrees(tree_t* simplified, tree_t* derivative)
{
    if (simplified != NULL)
    {
        if (simplified->root != NULL)
        {
            treeRecursiveDelete(simplified->root);
        }
        free(simplified);
    }

    if (derivative != NULL)
    {
        if (derivative->root != NULL)
        {
            treeRecursiveDelete(derivative->root);
        }
        free(derivative);
    }
}

void writeLaTeXFooter(FILE* tex_file)
{
    fprintf(tex_file, "\\end{document}\n");
}
