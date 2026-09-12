#ifndef PLUGIN_API_H
#define PLUGIN_API_H

#include "TreeNode.h"

typedef struct {
    void (*registerNative)(char *name, Node *(*func)(Node *args));
    Node *(*evaluate)(Node *node);
    Node *(*createLiteral)(int value);
    Node *(*createVariable)(char *varName);
    Node *(*createFunction)(Node *funcExpr, Node *args);
    Node *(*createList)(int value, Node *nextNode);
    Node *(*copyTree)(Node *root);
} VMAPI;

#endif
