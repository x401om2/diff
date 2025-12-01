#include "diff.h"
#include "proizv.h"
#include "toLatex.h"
#include "workWithFile.h"
#include "dump.h"
#include "simply.h"
#include <stdio.h>
#include <string.h>

void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename);

int main()
{
    initMathDebugHTM();

    VariableTable table = {};
    initVariableTable(&table, COUNT_OF_VARIABLES);

    tree_t* tree = loadMathTree("text.txt", &table);

    if (tree == NULL || tree->root == NULL)
    {
        printf("Ошибка: не удалось загрузить дерево из файла\n");
        return 1;
    }

    for (int i = 0; i < table.count; i++)
    {
        printf("Введите значение для %s: ", table.variables[i].name);
        scanf("%lf", &table.variables[i].value);
        table.variables[i].isDefined = true;
    }

    double original_result = countingTree(tree->root, &table);

    printf("\nСоздание комплексного отчета...\n");
    createComprehensiveReport(tree, &table, "comprehensive_report.tex");

    // Компилируем LaTeX в PDF
    printf("\nКомпиляция LaTeX в PDF...\n");
    system("pdflatex -interaction=nonstopmode comprehensive_report.tex");
    system("pdflatex -interaction=nonstopmode comprehensive_report.tex"); // Дважды для TOC
    printf("PDF отчет сохранен как comprehensive_report.pdf\n");

    // Дампы для отладки
    printf("\n=== Дампы для отладки ===\n");
    fullTreeDump(tree, "Исходное дерево");

    // Создаем упрощенное дерево для дампа
    tree_t* simplifiedTree = treeCtor();
    simplifiedTree->root = copyNode(tree->root);
    simplifiedTree->size = countTreeSize(simplifiedTree->root);
    simplifiedTree->root = simplification(simplifiedTree->root, simplifiedTree, NULL);
    fullTreeDump(simplifiedTree, "Упрощенное дерево");

    // Создаем производную для дампа
    tree_t* afterDiffTree = diffTree(tree, &table, "x");
    if (afterDiffTree != NULL && afterDiffTree->root != NULL) {
        afterDiffTree->root = simplification(afterDiffTree->root, afterDiffTree, NULL);
        fullTreeDump(afterDiffTree, "Дерево производной");
    }

    // Очистка памяти
    treeRecursiveDelete(tree->root);
    free(tree);

    treeRecursiveDelete(simplifiedTree->root);
    free(simplifiedTree);

    if (afterDiffTree != NULL && afterDiffTree->root != NULL) {
        treeRecursiveDelete(afterDiffTree->root);
        free(afterDiffTree);
    }

    deleteTable(&table);

    printf("\nПрограмма завершена успешно!\n");
    return 0;
}

void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename)
{
    FILE* tex_file = fopen(filename, "w");

    // Преамбула LaTeX
    fprintf(tex_file, "\\documentclass{article}\n");
    fprintf(tex_file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(tex_file, "\\usepackage{amsmath}\n");
    fprintf(tex_file, "\\usepackage[russian]{babel}\n");
    fprintf(tex_file, "\\usepackage{hyperref}\n");
    fprintf(tex_file, "\\title{Математический отчет}\n");
    fprintf(tex_file, "\\author{Автоматическая система}\n");
    fprintf(tex_file, "\\begin{document}\n");
    fprintf(tex_file, "\\maketitle\n");
    fprintf(tex_file, "\\tableofcontents\n");
    fprintf(tex_file, "\\newpage\n");

    // Глава 1: Исходные данные
    fprintf(tex_file, "\\section{Исходные данные}\n\n");
    formulaToLatex(original, tex_file, "Исходное выражение");

    fprintf(tex_file, "\\subsection{Таблица переменных}\n\n");
    variablesTableToLatex(table, tex_file);

    double original_result = countingTree(original->root, table);
    fprintf(tex_file, "Значение исходного выражения: $%.4f$\n\n", original_result);

    // Глава 2: Упрощения
    fprintf(tex_file, "\\section{Упрощение выражения}\n\n");

    tree_t* simplified = treeCtor();
    simplified->root = copyNode(original->root);

    fprintf(tex_file, "\\subsection{Поэтапное упрощение}\n\n");
    simplified->root = simplification(simplified->root, simplified, tex_file);

    fprintf(tex_file, "\\subsection{Финальное упрощенное выражение}\n\n");
    formulaToLatex(simplified, tex_file, "Упрощенное выражение");

    double simplified_result = countingTree(simplified->root, table);
    fprintf(tex_file, "Значение упрощенного выражения: $%.4f$\n\n", simplified_result);

    // Глава 3: Производная по x
    fprintf(tex_file, "\\section{Производная по переменной $x$}\n\n");

    tree_t* derivative = diffTree(simplified, table, "x");
    if (derivative != NULL && derivative->root != NULL) {
        fprintf(tex_file, "\\subsection{Исходная производная}\n\n");
        formulaToLatex(derivative, tex_file, "Производная");

        fprintf(tex_file, "\\subsection{Упрощенная производная}\n\n");
        derivative->root = simplification(derivative->root, derivative, tex_file);
        formulaToLatex(derivative, tex_file, "Упрощенная производная");

        double deriv_result = countingTree(derivative->root, table);
        fprintf(tex_file, "Значение производной: $%.4f$\n\n", deriv_result);

        treeRecursiveDelete(derivative->root);
        free(derivative);
    }

    // Глава 4: Итоги
    fprintf(tex_file, "\\section{Итоговые результаты}\n\n");
    fprintf(tex_file, "\\begin{tabular}{|l|c|}\n");
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "Параметр & Значение \\\\\n");
    fprintf(tex_file, "\\hline\n");
    fprintf(tex_file, "Исходное выражение & $%.4f$ \\\\\n", original_result);
    fprintf(tex_file, "Упрощенное выражение & $%.4f$ \\\\\n", simplified_result);

    // Повторно вычисляем производную для итогов
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

    // Очистка памяти
    treeRecursiveDelete(simplified->root);
    free(simplified);

    fprintf(tex_file, "\\end{document}\n");
    fclose(tex_file);
}
