#ifndef TOLATEX_FILE_H
#define TOLATEX_FILE_H

#include <stdio.h>
#include "diff.h"


void nodeToLatex(node_t* node, FILE* texFile);
void formulaToLatex(tree_t* tree, FILE* texFile, const char* title);

void formulaWithComputationToLatex(tree_t* tree, double valueOfX, FILE* texFile, const char* title);

void createComprehensiveReport(tree_t* original, const char* filename,  float plotMinX, float plotMaxX, double valueOfX);

void writeLaTeXPreamble(FILE* texFile);
void writeTitleAndTOC(FILE* texFile);


void cleanupTrees(tree_t* simplified, tree_t* derivative);
void writeLaTeXFooter(FILE* texFile);


void writeFinalResultsSection(tree_t* simplified, tree_t* derivative, FILE* texFile, double simplifiedValue, double derivativeValue, double valueOfX);


double writeOriginalDataSection(tree_t* original, FILE* texFile, double valueOfX);
tree_t* writeSimplificationSection(tree_t* original, FILE* texFile, double* simplifiedValue, float plotMinX, float plotMaxX, double valueOfX);

tree_t* writeDerivativeSection(tree_t* simplified, FILE* texFile, double* derivativeValue, float plotMinX, float plotMaxX, double valueOfX);

#endif
