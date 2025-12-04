#ifndef TOLATEX_FILE_H
#define TOLATEX_FILE_H

#include <stdio.h>
#include "diff.h"


void nodeToLatex(node_t* node, FILE* tex_file);
void formulaToLatex(tree_t* tree, FILE* tex_file, const char* title);
void formulaWithComputationToLatex(tree_t* tree, VariableTable* table, FILE* tex_file, const char* title);
void variablesTableToLatex(VariableTable* table, FILE* tex_file);

void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename);


#endif
