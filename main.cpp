#include "diff.h"
#include "proizv.h"
#include "toLatex.h"
#include "workWithFile.h"
#include "dump.h"
#include "simply.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    initMathDebugHTM();

    const char* inputFilename = "text.txt";
    if (argc > 1)
    {
        inputFilename = argv[1];
    }

    printf("обрабатываем файлик: %s\n", inputFilename);

    tree_t* tree = loadMathTree(inputFilename);
    if (tree == NULL || tree->root == NULL)
    {
        printf("не получилось грузануть дерево из файлика - %s\n", inputFilename);
        return 1;
    }

    double valueOfX = 0;

    getVariableValues(&valueOfX);

    float plotMinX = 0, plotMaxX = 0;
    getPlotRangeFromUser(&plotMinX, &plotMaxX);

    createComprehensiveReport(tree, "comprehensive_report.tex", plotMinX, plotMaxX , valueOfX);

    system("pdflatex -interaction=nonstopmode comprehensive_report.tex");

    printf("\nPDF отчет сохранен как comprehensive_report.pdf\n");

    treeRecursiveDelete(tree->root);
    free(tree);

    return 0;
}
