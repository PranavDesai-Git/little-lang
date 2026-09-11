#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "TreeNode.h"

Node *evaluate(Node *node);
Node *copyTree(Node *root);
Node *substitute(Node *root, char *paramName, Node *argValue);

#endif
