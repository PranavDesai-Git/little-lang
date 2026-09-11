#include "Allocator.h"
#include "Environment.h"
#include "Evaluator.h"
#include "GarbageCollector.h"
#include "NativeFuncs.h"
#include "TreeNode.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Starting GraphLang VM...\n");

    initAllocator();
    registerAllNatives();

    printf("Building AST for fib(n)...\n");

    Node *cond = createFunction("<", createVariable("n"), createLiteral(2));
    Node *trueBranch = createVariable("n");
    Node *falseBranch = createFunction(
        "+",
        createFunction(
            "fib", createFunction("-", createVariable("n"), createLiteral(1)),
            NULL),
        createFunction(
            "fib", createFunction("-", createVariable("n"), createLiteral(2)),
            NULL));

    Node *branches = allocNode();
    branches->type = LIST;
    branches->left = trueBranch;
    branches->right = falseBranch;

    Node *fibBody = createFunction("if", cond, branches);

    defineFunction("fib", "n", fibBody);

    Node *mainCall = createFunction("fib", createLiteral(10), NULL);
    defineVariable("main", mainCall);

    printf("Evaluating fib(10). This will spawn thousands of nodes...\n");
    Node *result = evaluate(mainCall);
    printf("\n=== RESULT: %d ===\n\n", result->data.literal);

    printf("Running Garbage Collector to clean up the thousands of dead "
           "trees...\n");
    markAll();
    sweep();
    printf("GC Complete! Dead nodes successfully recycled.\n");

    freeAllChunks();
    printf("VM Shutdown safely.\n");

    return 0;
}
