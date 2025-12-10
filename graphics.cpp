#include "diff.h"
#include "proizv.h"
#include "simply.h"
#include "toLatex.h"
#include "graphics.h"
#include <stdio.h>
#include <stdbool.h>


void addPlotToLatexDirect(tree_t* tree, VariableTable* table, FILE* tex_file, double x_min, double x_max, double step)
{

    if (tree == NULL || tex_file == NULL)
    {
        return;
    }

    // ищем переменную x в таблице - создали переменную для индекса и переменную для значения
    int x_idx = -1;
    double orig_x = 0;                                          // сохр исходное знач после построения графика
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->variables[i].name, "x") == 0)
        {
            x_idx = i;
            orig_x = table->variables[i].value;
            break;
        }
    }

    double y_min = 1e10, y_max = -1e10;               // оч big и оч small числа для поиска минимума и максимума
    int valid_points = 0;                            // счетчик корректных точек

    for (double x = x_min; x <= x_max; x += step)   // первым проходом вычисляем диапазон значений Y
    {
        if (x_idx != -1)
        {
            table->variables[x_idx].value = x;
        }
        double y = countingTree(tree->root, table);

        if (y == y)                                 // проверка на NaN
        {
            if (y < y_min)
            {
                y_min = y;
            }
            if (y > y_max)
            {
                y_max = y;
            }
            valid_points++;
        }
    }

    if (valid_points == 0)
    {
        y_min = -10;
        y_max = 10;
    }

    double y_range = y_max - y_min;
    if (y_range < 0.1)
    {
        y_min -= 5;
        y_max += 5;
    }
    else
    {
        y_min -= y_range * 0.1;
        y_max += y_range * 0.1;
    };

    fprintf(tex_file,
        "\\begin{center}\n"                                                       // центрируем график
        "\\begin{tikzpicture}\n"                                                  // основн графич контейнер
        "\\begin{axis}[\n"                                                        // спец окруж для графиков с осями (конкретная узкая задача - график)
        "    width=0.9\\textwidth,\n"
        "    height=0.5\\textwidth,\n"
        "    xlabel={$x$},\n"
        "    ylabel={$y$},\n"
        "    grid=major,\n"                                                       // основная сетка
        "    axis lines=middle,             %% Оси через центр\n"
        "    enlargelimits=true,            %% Автоматическое масштабирование\n"
        "    axis equal image=false,        %% Разные масштабы по осям\n"         // можно поставить true - будут одинаковые масштабы
        "    xmin=%.2f, xmax=%.2f,          %% Диапазон X\n"                      // заданные пределы осей х и у
        "    ymin=%.2f, ymax=%.2f,          %% Диапазон Y\n"
        "    ticklabel style={font=\\small} %% Размер подписей\n"
        "]\n"
        "\\addplot[blue, thick, smooth] coordinates {\n",                         // команда построения графика - синий цвет - толстая линия - сглаживание - данные заданы явными координатами
        x_min, x_max, y_min, y_max);

    for (double x = x_min; x <= x_max; x += step)
    {
        if (x_idx != -1)
        {
            table->variables[x_idx].value = x;
        }

        double y = countingTree(tree->root, table);

        if (y == y)
        {
            fprintf(tex_file, "    (%f, %f)\n", x, y);
        }
        else
        {
            fprintf(tex_file, "    (%f, nan)\n", x);
        }
    }

    fprintf(tex_file,
        "};\n"
        "\\end{axis}\n"
        "\\end{tikzpicture}\n"
        "\\end{center}\n\n");

    if (x_idx != -1)
    {
        table->variables[x_idx].value = orig_x;
    }
}


