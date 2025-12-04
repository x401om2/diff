#include "../INCLUDES/diff.h"
#include "../INCLUDES/proizv.h"
#include "../INCLUDES/toLatex.h"
#include "../INCLUDES/workWithFile.h"
#include "../INCLUDES/dump.h"
#include "../INCLUDES/simply.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{
    initMathDebugHTM();
    const char* inputFilename = "text.txt";                             // значение по умолчанию

    if (argc > 1)
    {
        inputFilename = argv[1];
    }

    printf("обработка файла: %s\n", inputFilename);

    VariableTable table = {0};
    initVariableTable(&table, COUNT_OF_VARIABLES);

    tree_t* tree = loadMathTree(inputFilename, &table);

    if (tree == NULL || tree->root == NULL)
    {
        printf("не удалось загрузить дерево из файла %s\n", inputFilename);
        return 1;
    }

    for (int i = 0; i < table.count; i++)                               // для всех переменных внутри считываемого текстовика
    {
        printf("введите значение для %s: ", table.variables[i].name);
        scanf("%lf", &table.variables[i].value);
        table.variables[i].isDefined = true;
    }

    createComprehensiveReport(tree, &table, "comprehensive_report.tex");        // comprehensive - всесторонний report

    system("pdflatex -interaction=nonstopmode comprehensive_report.tex");       // вызывает комп латех для генерации файла

    printf("\nPDF отчет сохранен как comprehensive_report.pdf\n");

    treeRecursiveDelete(tree->root);
    free(tree);
    deleteTable(&table);

    return 0;
}
