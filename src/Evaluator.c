#include "Environment.h"
#include "TreeNode.h"
#include "GarbageCollector.h"
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
        if (var->isFunc == 1 || var->isFunc == 2) {
            return node; // Return the function identifier node as-is!
        }
        return evaluate(var->val.node);
    }

    case FUNCTION: { // This is an APPLICATION node
        Node *funcNode = evaluate(node->left); // Get the function to run
        if (funcNode->type != VARIABLE) {
            printf("Runtime Error: Not a function!\n");
            exit(1);
        }
        
        EnvEntry *func = getEnvEntry(funcNode->data.var);
        if (func == NULL) {
            printf("Runtime Error: Undefined function '%s'\n", funcNode->data.var);
            exit(1);
        }

        Node *result;

        if (func->isFunc == 1) { // Native C Function
            Func eval = func->val.func;
            result = eval(node->right); // Args are in right

        } else if (func->isFunc == 2) { // User-Defined Function
            Node *bodyClone = copyTree(func->val.node);
            pushRoot(bodyClone);
            
            Node *currParam = func->params;
            Node *currArg = node->right; // Args are in right
            
            while (currParam != NULL && currArg != NULL) {
                if (currParam->left && currParam->left->type == VARIABLE) {
                    bodyClone = substitute(bodyClone, currParam->left->data.var, currArg->left);
                }
                currParam = currParam->right;
                currArg = currArg->right;
            }
            
            result = evaluate(bodyClone);
            popRoot();

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
