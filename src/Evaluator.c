#include "Environment.h"
#include "TreeNode.h"
#include "GarbageCollector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *evaluate(Node *node, LocalEnv *env) {
    if (node == NULL)
        return NULL;

    switch (node->type) {
    case LITERAL:
        return node;

    case VARIABLE: {
        LocalEnv *temp = env;
        char *varName = node->data.var;
        while (temp != NULL) {
            if (strcmp(temp->varName, varName) == 0) {
                Node *evaluatedArg = evaluate(temp->value, env);
                temp->value = evaluatedArg; // Memoize in the environment!
                return evaluatedArg;
            }
            temp = temp->next;
        }
        EnvEntry *var = getEnvEntry(varName);
        if (var == NULL) {
            printf("Runtime Error: Undefined variable '%s'\n", node->data.var);
            exit(1);
        }
        if (var->isFunc == 1 || var->isFunc == 2) {
            return node; // Return the function identifier node as-is!
        }

        return evaluate(var->val.node, env);
    }

    case FUNCTION: { // This is an APPLICATION node
        Node *funcNode = evaluate(node->left, env); // Get the function to run
        if (funcNode->type != VARIABLE) {
            printf("Runtime Error: Not a function!\n");
            exit(1);
        }

        EnvEntry *func = getEnvEntry(funcNode->data.var);
        if (func == NULL) {
            printf("Runtime Error: Undefined function '%s'\n",
                   funcNode->data.var);
            exit(1);
        }

        Node *result;

        if (func->isFunc == 1) { // Native C Function
            Func eval = func->val.func;
            result = eval(node->right, env); // Args are in right

        } else if (func->isFunc == 2) { // User-Defined Function
            LocalEnv newEnv;
            newEnv.varName = func->params->left->data.var;
            newEnv.value = evaluate(node->right->left, env);
            pushRoot(newEnv.value); // PROTECT FROM GC!
            newEnv.next = env;
            result = evaluate(func->val.node, &newEnv);
            popRoot(); // UNPROTECT
        } else {
            printf("Runtime Error: '%s' is not a function!\n", func->key);
            exit(1);
        }

        return result;
    }

    case LIST:
    default:
        return node;
    }
}
