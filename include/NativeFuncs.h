#ifndef NATIVEFUNCS_H
#define NATIVEFUNCS_H
#include "TreeNode.h"

Node *nativeAdd(Node *args);
Node *nativeSub(Node *args);
Node *nativeMul(Node *args);
Node *nativeDiv(Node *args);
Node *nativeIf(Node *args);
Node *nativeLessThan(Node *args);
Node *nativeGreaterThan(Node *args);
Node *nativeEquals(Node *args);
Node *nativeAnd(Node *args);
Node *nativeOr(Node *args);
Node *nativeNot(Node *args);

void registerNative(char *name, Node *(*funcPtr)(Node *));
void registerAllNatives(void);

#endif
