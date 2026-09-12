#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "TreeNode.h"
#include "Environment.h"

Node *evaluate(Node *node, LocalEnv *env);
Node *copyTree(Node *root);
Node *substitute(Node *root, char *paramName, Node *argValue);

#endif
