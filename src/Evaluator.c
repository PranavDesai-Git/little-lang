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

        Func eval = func->val.func;
        Node *result = eval(evaluate(node->left), evaluate(node->right));

        node->type = LITERAL;
        node->data = result->data;
        node->left = NULL;
        node->right = NULL;

        return node;
    }

    case LIST:
    default:
        return node;
    }
}
