#ifndef TOLATEX_FILE_H
#define TOLATEX_FILE_H

#include <stdio.h>
#include "diff.h"


void nodeToLatex(node_t* node, FILE* tex_file);
void treeToLatexFile(tree_t* tree, VariableTable* table, const char* filename);


#endif
