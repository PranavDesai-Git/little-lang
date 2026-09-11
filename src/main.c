#include "Allocator.h"
#include "Environment.h"
#include "Evaluator.h"
#include "GarbageCollector.h"
#include "NativeFuncs.h"
#include "TreeNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    printf("Starting GraphLang VM...\n");

    initAllocator();
    registerAllNatives();

    printf("Building AST for sum(n)...\n");

    // cond: n < 1
    Node *cond = createFunction(createVariable("<"), createArgs2(createVariable("n"), createLiteral(1)));
    
    // trueBranch: 0
    Node *trueBranch = createLiteral(0);
    
    // falseBranch: n + sum(n - 1)
    Node *sum_n_minus_1 = createFunction(
        createVariable("sum"),
        createArgs1(createFunction(
            createVariable("-"),
            createArgs2(createVariable("n"), createLiteral(1)))));
    Node *falseBranch = createFunction(createVariable("+"), createArgs2(createVariable("n"), sum_n_minus_1));

    // if (n < 1) 0 else n + sum(n - 1)
    Node *sumBody = createFunction(createVariable("if"), createArgs3(cond, trueBranch, falseBranch));

    // Create the parameter list for sum: [n]
    Node *paramsList = createList(0, NULL);
    paramsList->left = createVariable("n");
    defineFunction("sum", paramsList, sumBody);

    // sum(500)
    Node *mainCall = createFunction(createVariable("sum"), createArgs1(createLiteral(500)));
    
    defineVariable("main", mainCall);

    printf("Evaluating sum(500)... \n");
    enableGC();
    
    clock_t start = clock();
    Node *result = evaluate(mainCall);
    clock_t end = clock();
    
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\n=== RESULT: %d ===\n", result->data.literal);
    printf("=== TIME: %f seconds ===\n\n", time_spent);

    printf("Running Final GC Pass...\n");
    markAll();
    sweep();
    printf("GC Complete! Dead nodes successfully recycled.\n");

    freeAllChunks();
    printf("VM Shutdown safely.\n");

    return 0;
}
