#include "Environment.h"
#include "TreeNode.h"
#include <stdio.h>
#include <stdlib.h>

Node *evaluate(Node *node) {
    if (node == NULL)
        return NULL;

    switch (node->type) {
    case LITERAL:
        return node;

    case VARIABLE: {
        EnvEntry *var = getEnvEntry(node->data.var);
        if (var == NULL) {
            printf("Runtime Error: Undefined variable '%s'\n", node->data.var);
            exit(1);
        }
        return evaluate(var->val.node);
    }

    case FUNCTION: {
        EnvEntry *func = getEnvEntry(node->data.func);
        if (func == NULL) {
            printf("Runtime Error: Undefined function '%s'\n", node->data.func);
            exit(1);
        }

        Node *result;

        if (func->isFunc == 1) {
            Func eval = func->val.func;
            result = eval(node->left);

        } else if (func->isFunc == 2) {
            Node *bodyClone = copyTree(func->val.node);
            
            Node *currParam = func->params;
            Node *currArg = node->left; // arguments are passed as a LIST in left branch
            
            while (currParam != NULL && currArg != NULL) {
                if (currParam->left && currParam->left->type == VARIABLE) {
                    bodyClone = substitute(bodyClone, currParam->left->data.var, currArg->left);
                }
                currParam = currParam->right;
                currArg = currArg->right;
            }
            
            result = evaluate(bodyClone);

        } else {
            printf("Runtime Error: '%s' is not a function!\n", func->key);
            exit(1);
        }

        node->type = result->type;
        node->data = result->data;
        node->left = result->left;
        node->right = result->right;

        return node;
    }

    case LIST:
    default:
        return node;
    }
}
