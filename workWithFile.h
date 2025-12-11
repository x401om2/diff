#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "diff.h"


typedef struct {
    const char* s;                // указатель на текущую позицию в строке
} parserState;


tree_t* loadMathTree(const char* filename);


node_t* getNumber(parserState* st);
node_t* getExpression(parserState* st);
node_t* getTerm(parserState* st);
node_t* getPrimary(parserState* st);
node_t* getFunction(parserState* st);
node_t* getGrammar(parserState* st);
node_t* getPower(parserState* st);


#endif
