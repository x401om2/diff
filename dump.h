#ifndef DUMP_H
#define DUMP_H

#include "diff.h"
#include "workWithFile.h"
#include <stdio.h>


void fullTreeDump(const tree_t* tree, const char* title);
void treeGraphDump(const tree_t* tree, const char* filename);
void generateDotNodes(FILE* dotFile, const node_t* node, int* nodeCounter);
const char* getNodeValueString(const node_t* node);

void initMathDebugHTM();
void appendTreeHTMReport(const tree_t* tree, const char* title, int dumpCounter, const char* pngFilename);



#endif
