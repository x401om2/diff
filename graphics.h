#ifndef GRA_H
#define GRA_H

void addPlotToLatexDirect(tree_t* tree, FILE* texFile, double xMin, double xMax, double step, double valueOfX);
void calculateYRangeSimple(const tree_t* tree, double xMin, double xMax, double step, double* yMin, double* yMax, double valueOfX);
void writePlotHeaderSimple(FILE* texFile, double xMin, double xMax, double yMin, double yMax);
void writePlotPoints(const tree_t* tree, FILE* texFile, double xMin, double xMax, double step, double valueOfX);



#endif


