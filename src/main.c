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

    Node *cond = createFunction(createVariable("<"), createArgs2(createVariable("n"), createLiteral(2)));
    Node *trueBranch = createVariable("n");
    
    // fib(n-1)
    Node *fib_1 = createFunction(createVariable("fib"), createArgs1(createFunction(createVariable("-"), createArgs2(createVariable("n"), createLiteral(1)))));

    // fib(n-2)
    Node *fib_2 = createFunction(createVariable("fib"), createArgs1(createFunction(createVariable("-"), createArgs2(createVariable("n"), createLiteral(2)))));

    // fib(n-1) + fib(n-2)
    Node *falseBranch = createFunction(createVariable("+"), createArgs2(fib_1, fib_2));

    Node *fibBody = createFunction(createVariable("if"), createArgs3(cond, trueBranch, falseBranch));

    // Create the parameter list for fib: [n]
    Node *paramsList = createList(0, NULL);
    paramsList->left = createVariable("n");
    defineFunction("fib", paramsList, fibBody);

    // Create the argument list for mainCall: [10]
    Node *mainCall = createFunction(createVariable("fib"), createArgs1(createLiteral(10)));
    
    defineVariable("main", mainCall);

    printf("Evaluating fib(10). This will spawn thousands of nodes...\n");
    Node *result = evaluate(mainCall);
    printf("\n=== RESULT: %d ===\n\n", result->data.literal);

    printf("Running Garbage Collector to clean up the thousands of dead trees...\n");
    markAll();
    sweep();
    printf("GC Complete! Dead nodes successfully recycled.\n");

    freeAllChunks();
    printf("VM Shutdown safely.\n");

    return 0;
}
