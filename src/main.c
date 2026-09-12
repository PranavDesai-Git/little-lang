#include "Allocator.h"
#include "Environment.h"
#include "Evaluator.h"
#include "GarbageCollector.h"
#include "PluginAPI.h"
#include "TreeNode.h"
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>
#include <string.h>

void loadPlugin(const char *path, VMAPI api) {
    void *handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        printf("FFI Error: Failed to load plugin '%s'\nReason: %s\n", path, dlerror());
        return;
    }
    dlerror();
    void (*initPlugin)(VMAPI);
    *(void **)(&initPlugin) = dlsym(handle, "initPlugin");
    const char *err = dlerror();
    if (err != NULL) {
        printf("FFI Error: Could not find 'initPlugin' inside '%s'\nReason: %s\n", path, err);
        dlclose(handle);
        return;
    }
    initPlugin(api);
    printf("Successfully loaded plugin: %s\n", path);
}

int main(void) {
    // TEST PROGRAM ADDS SHIT
    VMAPI api = {.registerNative = registerNative,
                 .evaluate = evaluate,
                 .createLiteral = createLiteral,
                 .createVariable = createVariable,
                 .createFunction = createFunction,
                 .createList = createList,
                 .copyTree = copyTree};
    // hardcoded for now
    loadPlugin("./out/CoreMath.so", api);

    printf("Starting GraphLang VM...\n");

    initAllocator();

    printf("Building AST for sum(n)...\n");

    // cond: n < 1
    Node *cond =
        createFunction(createVariable("<"),
                       createArgs2(createVariable("n"), createLiteral(1)));

    // trueBranch: 0
    Node *trueBranch = createLiteral(0);

    // falseBranch: n + sum(n - 1)
    Node *sum_n_minus_1 = createFunction(
        createVariable("sum"),
        createArgs1(createFunction(
            createVariable("-"),
            createArgs2(createVariable("n"), createLiteral(1)))));
    Node *falseBranch = createFunction(
        createVariable("+"), createArgs2(createVariable("n"), sum_n_minus_1));

    // ? (cond) 0 else n + sum(n - 1)
    Node *sumBody = createFunction(createVariable("?"),
                                   createArgs3(cond, trueBranch, falseBranch));

    // Create the parameter list for sum: [n]
    Node *paramsList = createList(0, NULL);
    paramsList->left = createVariable("n");
    defineFunction("sum", paramsList, sumBody);

    // sum(500)
    Node *mainCall =
        createFunction(createVariable("sum"), createArgs1(createLiteral(500)));

    defineVariable("main", mainCall);

    printf("Evaluating sum(500)... \n");
    enableGC();

    clock_t start = clock();
    Node *result = evaluate(mainCall, NULL);
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
