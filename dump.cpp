#include "dump.h"
#include "diff.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const char* getNodeValueString(const node_t* node)
{
    static char buffer[MAX_NAME_LEN] = {0};

    if (node == NULL) return "NULL";

    switch (node->type) {
        case NUM:
            snprintf(buffer, sizeof(buffer), "%.2f", node->object.constant);
            break;
        case OP:
            switch (node->object.operation) {
                case ADD: return "+";
                case SUB: return "-";
                case MUL: return "*";
                case DIV: return "/";
                case SIN: return "sin";
                case ARCSIN: return "arcsin";
                case COS: return "cos";
                case ARCCOS: return "arccos";
                case TG: return "tg";
                case ARCTG: return "arctg";
                case CTG: return "ctg";
                case ARCCTG: return "arcctg";
                case RAIZE: return "^";
                case SQRT: return "sqrt";
                case SH: return "sh";
                case CH: return "ch";
                case TH: return "th";
                case CTH: return "cth";
                case HZ_OPERATION: return "?";
                default: return "?";
            }
            break;
        case VAR:
            snprintf(buffer, sizeof(buffer), "%s", node->object.var);
            break;
        default:
            return "UNKNOWN";
    }
    return buffer;
}


void generateDotNodes(FILE* dotFile, const node_t* node, int* nodeCounter)
{
    if (node == NULL) return;

    int currentId = *nodeCounter;

    const char* value = getNodeValueString(node);
    const char* nodeType = "";
    const char* fillColor = "white";

    switch (node->type) {
        case NUM:
            nodeType = "NUM";
            fillColor = "lightgreen";
            break;
        case OP:
            nodeType = "OP";
            fillColor = "lightblue";
            break;
        case VAR:
            nodeType = "VAR";
            fillColor = "lightyellow";
            break;
    }

    fprintf(dotFile, "    node%d [label=\"{%s | %s}\", style=filled, fillcolor=%s];\n", currentId, nodeType, value, fillColor);

    (*nodeCounter)++;

    if (node->left != NULL)
    {
        int leftId = *nodeCounter;
        generateDotNodes(dotFile, node->left, nodeCounter);
        fprintf(dotFile, "    node%d -> node%d [label=\"left\"];\n", currentId, leftId);
    }

    if (node->right != NULL)
    {
        int rightId = *nodeCounter;
        generateDotNodes(dotFile, node->right, nodeCounter);
        fprintf(dotFile, "    node%d -> node%d [label=\"right\"];\n", currentId, rightId);
    }
}


void treeGraphDump(const tree_t* tree, const char* filename)
{
    if (tree == NULL) return;

    char dotFilename[256] = {0};
    snprintf(dotFilename, sizeof(dotFilename), "IMAGES/%s.dot", filename);

    FILE* dotFile = fopen(dotFilename, "w");
    if (!dotFile) return;

    fprintf(dotFile, "digraph ExpressionTree {\n");
    fprintf(dotFile, "    rankdir=TB;\n");
    fprintf(dotFile, "    node [shape=record, fontname=\"Arial\"];\n");
    fprintf(dotFile, "    edge [fontsize=10];\n\n");

    if (tree->root == NULL)
    {
        fprintf(dotFile, "    empty [label=\"EMPTY\\nTREE\"];\n");
    } else {
        int nodeCounter = 0;
        generateDotNodes(dotFile, tree->root, &nodeCounter);
    }

    fprintf(dotFile, "}\n");
    fclose(dotFile);

    char pngFilename[256] = {0};
    snprintf(pngFilename, sizeof(pngFilename), "IMAGES/%s.png", filename);

    char command[512] = {0};
    snprintf(command, sizeof(command), "dot -Tpng \"IMAGES/%s.dot\" -o \"IMAGES/%s.png\"", filename, filename);
    system(command);

    remove(dotFilename);

    printf("Expression tree dump saved to %s\n", pngFilename);
}


void fullTreeDump(const tree_t* tree, const char* title)
{
    if (tree == NULL) return;

    static int dumpCounter = 0;
    dumpCounter++;

    char filename[256] = {0};
    snprintf(filename, sizeof(filename), "expr_dump_%d", dumpCounter);

    printf("=== %s (Dump #%d) ===\n", title, dumpCounter);
    printf("Tree size: %zu\n", tree->size);

    treeGraphDump(tree, filename);

    appendTreeHTMReport(tree, title, dumpCounter, filename);
}


void initMathDebugHTM()
{
    FILE* htm = fopen("math_debug.html", "w");
    if (htm) {
        fprintf(htm, "<!DOCTYPE html>\n");
        fprintf(htm, "<html>\n");
        fprintf(htm, "<head><title>Math Tree Dumps</title></head>\n");
        fprintf(htm, "<body>\n");
        fprintf(htm, "<h1>Math Tree Dumps</h1>\n");
        fclose(htm);
    }
}


void appendTreeHTMReport(const tree_t* tree, const char* title, int dumpCounter, const char* filename)
{
    FILE* htm = fopen("math_debug.html", "a");
    if (!htm) return;

    fprintf(htm, "<div class='dump'>\n");
    fprintf(htm, "<h2>%s</h2>\n", title);
    fprintf(htm, "<p><b>Dump #:</b> %d</p>\n", dumpCounter);

    if (tree) {
        fprintf(htm, "<p><b>Tree size:</b> %zu</p>\n", tree->size);
        fprintf(htm, "<p><b>Root address:</b> %p</p>\n", (void*)tree->root);
    }

    fprintf(htm, "<img class='tree-img' src='IMAGES/%s.png' alt='Tree visualization' width='600'>\n", filename);

    fprintf(htm, "<pre>\n");
    if (tree && tree->root) {
        fprintf(htm, "Tree structure loaded successfully\n");
    } else {
        fprintf(htm, "Empty tree or loading failed\n");
    }
    fprintf(htm, "</pre>\n");

    fprintf(htm, "</div>\n\n");
    fclose(htm);
}
