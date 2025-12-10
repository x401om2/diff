#ifndef TOLATEX_FILE_H
#define TOLATEX_FILE_H

#include <stdio.h>
#include "diff.h"


void nodeToLatex(node_t* node, FILE* tex_file);
void formulaToLatex(tree_t* tree, FILE* tex_file, const char* title);
void formulaWithComputationToLatex(tree_t* tree, VariableTable* table, FILE* tex_file, const char* title);
void variablesTableToLatex(VariableTable* table, FILE* tex_file);

void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename,  float plotMinX, float plotMaxX);

void writeLaTeXPreamble(FILE* tex_file);
void writeTitleAndTOC(FILE* tex_file);
double writeOriginalDataSection(tree_t* original, VariableTable* table, FILE* tex_file);
tree_t* writeSimplificationSection(tree_t* original, VariableTable* table, FILE* tex_file, double* simplified_value, float plotMinX, float plotMaxX);
tree_t* writeDerivativeSection(tree_t* simplified, VariableTable* table, FILE* tex_file, double* derivative_value, float plotMinX, float plotMaxX);
void writeFinalResultsSection(tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* tex_file, double simplified_value, double derivative_value);
void writeSummaryTable(tree_t* original, tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* tex_file, double original_value, double simplified_value, double derivative_value);
void cleanupTrees(tree_t* simplified, tree_t* derivative);
void writeLaTeXFooter(FILE* tex_file);


#endif
