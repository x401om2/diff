#include "diff.h"
#include "proizv.h"
#include "simply.h"
#include "toLatex.h"
#include "graphics.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// void addPlotToLatexDirect(tree_t* tree, VariableTable* table, FILE* texFile, double xMin, double xMax, double step)
// {
//     if (tree == NULL || texFile == NULL)
//     {
//         return;
//     }
//
//     int indexX = -1;
//     double originalX= 0;
//
//     for (int i = 0; i < table->count; i++)
//     {
//         if (strcmp(table->variables[i].name, "x") == 0)
//         {
//             indexX = i;
//             originalX = table->variables[i].value;
//             break;
//         }
//     }
//
//     double yMin = 0, yMax = 0;
//
//     calculateYRangeSimple(tree, table, indexX, xMin, xMax, step, &yMin, &yMax);
//
//     writePlotHeaderSimple(texFile, xMin, xMax, yMin, yMax);
//
//     writePlotPoints(tree, table, texFile, indexX, xMin, xMax, step);
//
//     fprintf(texFile, "};\n\\end{axis}\n\\end{tikzpicture}\n\\end{center}\n\n");
//
//     if (indexX != -1)                                                               // если изменяли х то вернем исходное
//     {
//         table->variables[indexX].value = originalX;
//     }
// }

void addPlotToLatexDirect(tree_t* tree, FILE* texFile, double xMin, double xMax, double step, double valueOfX)
{
    if (tree == NULL || texFile == NULL)
    {
        return;
    }

    double originalX = valueOfX;

    double yMin = 0, yMax = 0;

    calculateYRangeSimple(tree, xMin, xMax, step, &yMin, &yMax, valueOfX);            //table

    writePlotHeaderSimple(texFile, xMin, xMax, yMin, yMax);

    writePlotPoints(tree, texFile, xMin, xMax, step, valueOfX);                       // table

    fprintf(texFile, "};\n\\end{axis}\n\\end{tikzpicture}\n\\end{center}\n\n");
}



void calculateYRangeSimple(const tree_t* tree, double xMin, double xMax, double step, double* yMin, double* yMax, double valueOfX)       //VariableTable* table
{
    *yMin = 1e10;
    *yMax = -1e10;

    for (double x = xMin; x <= xMax; x += step)                                    // цикл по всем знач с шагом step
    {
        valueOfX = x;

        double y = countingTree(tree->root, valueOfX);

        if (!isnan(y))
        {
            if (y < *yMin)
            {
                *yMin = y;
            }
            if (y > *yMax)
            {
                *yMax = y;
            }
        }
    }
    double yRange = *yMax - *yMin;

    if (yRange < 0.1)                                                             // если диапазон оч маленький то добавляем по 5 с каждой стороны
    {
        *yMin -= 5;
        *yMax += 5;
    }
    else
    {
        *yMin -= yRange * 0.1;                                                     // иначе по 10% от краев убираем чтобы график по краям не врезался
        *yMax += yRange * 0.1;
    }
}

void writePlotHeaderSimple(FILE* texFile, double xMin, double xMax, double yMin, double yMax)
{
    fprintf(texFile,
        "\\begin{center}\n"
        "\\begin{tikzpicture}\n"
        "\\begin{axis}[\n"                                                      // начало системы координат
        "    width=0.9\\textwidth,\n"
        "    height=0.5\\textwidth,\n"
        "    xlabel={$x$},\n"                                                   // подписи осей
        "    ylabel={$y$},\n"
        "    grid=major,\n"
        "    axis lines=middle,\n"                                              // оси через центр
        "    enlargelimits=true,\n"
        "    axis equal image=false,\n"                                         // масштабы по осям - РАЗНЫЕ (false)
        "    xmin=%.2f, xmax=%.2f,\n"
        "    ymin=%.2f, ymax=%.2f,\n"
        "    ticklabel style={font=\\small}\n"
        "]\n"
        "\\addplot[blue, thick, smooth] coordinates {\n", xMin, xMax, yMin, yMax);  // команда построения
}

void writePlotPoints(const tree_t* tree, FILE* texFile, double xMin, double xMax, double step, double valueOfX) // VariableTable* table
{
    for (double x = xMin; x <= xMax; x += step)
    {
        valueOfX = x;

        double y = countingTree(tree->root, valueOfX);

        if (!isnan(y))
        {
            fprintf(texFile, "    (%f, %f)\n", x, y);
        }
        else
        {
            fprintf(texFile, "    (%f, nan)\n", x);
        }
    }
}

