#include "workWithFile.h"
#include "diff.h"
#include <ctype.h>
// #include "akinator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



node_t* loadNodeMathExpressionFromBuffer(const char* buffer, int* pos, tree_t* tree, VariableTable* table)
{
    if (buffer == NULL || pos == NULL) return NULL;

    skipWhitespaces(buffer, pos);

    if (strncmp(&buffer[*pos], "nil", 3) == 0)
    {
        (*pos) += 3;
        return NULL;
    }

    if (buffer[*pos] == '(')
    {
        (*pos)++;
        skipWhitespaces(buffer, pos);

        if (isdigit(buffer[*pos]) || (buffer[*pos] == '-' && isdigit(buffer[(*pos) + 1])))
        {
            char numberStr[256] = {0};
            int i = 0;

            if (buffer[*pos] == '-')
            {
                numberStr[i++] = '-';
                (*pos)++;
            }

            while (isdigit(buffer[*pos]) || buffer[*pos] == '.')
            {
                numberStr[i++] = buffer[*pos];
                (*pos)++;
            }
            numberStr[i] = '\0';

            node_t* node = createTypedNode(NUM, numberStr, NULL, NULL);

            skipWhitespaces(buffer, pos);

            node->left = loadNodeMathExpressionFromBuffer(buffer, pos, tree, table);
            skipWhitespaces(buffer, pos);
            node->right = loadNodeMathExpressionFromBuffer(buffer, pos, tree, table);
            skipWhitespaces(buffer, pos);

            if (buffer[*pos] == ')')
            {
                (*pos)++;
            }

            return node;
        }

        if (buffer[*pos] == '+' || (buffer[*pos] == '-' && !isdigit(buffer[*pos+1])) ||
            buffer[*pos] == '*' || buffer[*pos] == '/' || buffer[*pos] == '^')
        {
            char temp[2] = "";
            temp[0] = buffer[*pos];

            (*pos)++;
            skipWhitespaces(buffer, pos);

            node_t* left = loadNodeMathExpressionFromBuffer(buffer, pos, tree, table);
            skipWhitespaces(buffer, pos);

            node_t* right = loadNodeMathExpressionFromBuffer(buffer, pos, tree, table);

            node_t* node = createTypedNode(OP, temp, left, right);

            skipWhitespaces(buffer, pos);

            if (buffer[*pos] == ')')
            {
                (*pos)++;
            }

            return node;
        }

        if (isalpha(buffer[*pos]))
        {
            char alphaStr[256] = {0};
            int i = 0;

            while (isalpha(buffer[*pos]))
            {
                alphaStr[i] = buffer[*pos];
                (*pos)++;
                i++;
            }
            alphaStr[i] = '\0';

            if (strcmp(alphaStr, "sin") == 0 || strcmp(alphaStr, "cos") == 0 ||
                strcmp(alphaStr, "tg") == 0 || strcmp(alphaStr, "arcsin") == 0 ||
                strcmp(alphaStr, "arccos") == 0 || strcmp(alphaStr, "arctg") == 0 ||
                strcmp(alphaStr, "ln") == 0 || strcmp(alphaStr, "ctg") == 0 ||
                strcmp(alphaStr, "arcctg") == 0)
            {
                node_t* node = createTypedNode(OP, alphaStr, NULL, NULL);

                skipWhitespaces(buffer, pos);
                node->left = loadNodeMathExpressionFromBuffer(buffer, pos, tree, table);
                node->right = NULL;

                skipWhitespaces(buffer, pos);

                if (buffer[*pos] == ')')
                {
                    (*pos)++;
                }

                return node;
            }

            node_t* node = createTypedNode(VAR, alphaStr, NULL, NULL);
            findVarInTable(table, alphaStr);

            skipWhitespaces(buffer, pos);

            node->left = loadNodeMathExpressionFromBuffer(buffer, pos, tree, table);
            skipWhitespaces(buffer, pos);
            node->right = loadNodeMathExpressionFromBuffer(buffer, pos, tree, table);
            skipWhitespaces(buffer, pos);

            if (buffer[*pos] == ')')
            {
                (*pos)++;
            }

            return node;
        }
    }

    printf("Ошибка: нераспознанный символ '%c' на позиции %d\n", buffer[*pos], *pos);
    return NULL;
}


tree_t* loadMathTree(const char* filename, VariableTable* table)
{
    FILE* file = fopen(filename, "r");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

    if (bytes_read != file_size)
    {
        printf("Ошибка: прочитано %zu байт вместо %ld\n", bytes_read, file_size);
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(file);

    tree_t* tree = treeCtor();
    if (tree == NULL)
    {
        printf("Ошибка: не удалось создать дерево\n");
        free(buffer);
        return NULL;
    }

    int current_position = 0;

    tree->root = loadNodeMathExpressionFromBuffer(buffer, &current_position, tree, table);

    free(buffer);

    setParentLinks(tree->root, NULL);
    tree->size = countTreeSize(tree->root);

    return tree;
}

