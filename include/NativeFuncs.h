#ifndef NATIVEFUNCS_H
#define NATIVEFUNCS_H
#include "TreeNode.h"

Node *nativeAdd(Node *left, Node *right);
Node *nativeSub(Node *left, Node *right);
Node *nativeMul(Node *left, Node *right);
Node *nativeDiv(Node *left, Node *right);
Node *nativeIf(Node *condition, Node *branches);
Node *nativeLessThan(Node *left, Node *right);
Node *nativeGreaterThan(Node *left, Node *right);
Node *nativeEquals(Node *left, Node *right);
Node *nativeAnd(Node *left, Node *right);
Node *nativeOr(Node *left, Node *right);
Node *nativeNot(Node *left, Node *right);

void registerNative(char *name, Node *(*funcPtr)(Node *, Node *));
void registerAllNatives(void);

#endif
