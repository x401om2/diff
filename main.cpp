#include "diff.h"
#include "proizv.h"
#include "toLatex.h"
#include "workWithFile.h"
#include "dump.h"
#include <stdio.h>
#include <string.h>

int main()
{
    VariableTable table;
    initVariableTable(&table, COUNT_OF_VARIABLES);

    tree_t* tree = loadMathTree("text.txt", &table);


    // fullTreeDump(tree, "Math Expression Tree");


    for (int i = 0; i < table.count; i++)
    {
        printf("Введите значение для %s: ", table.variables[i].name);
        scanf("%lf", &table.variables[i].value);
        table.variables[i].isDefined = true;
    }

    if (tree != NULL && tree->root != NULL)
    {
        printf("Original tree: ");
        printAkinatorTree(tree->root);
        printf("\n");

        fullTreeDump(tree, "Math Expression Tree");
    } else {
        printf("Failed to load tree\n");
    }

    double penis = 0;

    penis = countingTree(tree->root, &table);
    printf("\nответом является число = %f\n", penis);


    tree_t* afterDiffTree = diffTree(tree, &table, "x");

    printf("Derivative tree: ");
    printAkinatorTree(afterDiffTree->root);
    printf("\n");
    fullTreeDump(afterDiffTree, "Derivative Tree");


    // treeToLatexFile(tree, &table, "expression.tex");
    return 0;
}
