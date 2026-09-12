#include "PluginAPI.h"
#include <stddef.h>
#include <stdio.h>

static VMAPI vm;

Node *nativeAdd(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left, env);
    Node *r = vm.evaluate(right, env);
    return vm.createLiteral(l->data.literal + r->data.literal);
}

Node *nativeSub(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left, env);
    Node *r = vm.evaluate(right, env);
    return vm.createLiteral(l->data.literal - r->data.literal);
}

Node *nativeMul(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    return vm.createLiteral(vm.evaluate(left, env)->data.literal *
                            vm.evaluate(right, env)->data.literal);
}

Node *nativeDiv(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    return vm.createLiteral(vm.evaluate(left, env)->data.literal /
                            vm.evaluate(right, env)->data.literal);
}

Node *nativeLessThan(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left, env);
    Node *r = vm.evaluate(right, env);
    return vm.createLiteral(l->data.literal < r->data.literal ? 1 : 0);
}

Node *nativeGreaterThan(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left, env);
    Node *r = vm.evaluate(right, env);
    return vm.createLiteral(l->data.literal > r->data.literal ? 1 : 0);
}

Node *nativeEquals(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left, env);
    Node *r = vm.evaluate(right, env);
    return vm.createLiteral(l->data.literal == r->data.literal ? 1 : 0);
}

Node *nativeAnd(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left, env);
    if (l->data.literal == 0)
        return vm.createLiteral(0);
    Node *r = vm.evaluate(right, env);
    return vm.createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeOr(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left, env);
    if (l->data.literal != 0)
        return vm.createLiteral(1);
    Node *r = vm.evaluate(right, env);
    return vm.createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeNot(Node *args, struct LocalEnv *env) {
    Node *left = args->left;
    Node *l = vm.evaluate(left, env);
    return vm.createLiteral(l->data.literal == 0 ? 1 : 0);
}

Node *nativeIf(Node *args, struct LocalEnv *env) {
    Node *condition = args->left;
    Node *trueBranch = args->right->left;
    Node *falseBranch = args->right->right->left;

    Node *cond = vm.evaluate(condition, env);
    if (cond->data.literal == 1) {
        return vm.evaluate(trueBranch, env);
    } else {
        return vm.evaluate(falseBranch, env);
    }
}

void initPlugin(VMAPI providedApi) {
    vm = providedApi;
    vm.registerNative("+", nativeAdd);
    vm.registerNative("-", nativeSub);
    vm.registerNative("*", nativeMul);
    vm.registerNative("/", nativeDiv);
    vm.registerNative("<", nativeLessThan);
    vm.registerNative(">", nativeGreaterThan);
    vm.registerNative("==", nativeEquals);
    vm.registerNative("&&", nativeAnd);
    vm.registerNative("||", nativeOr);
    vm.registerNative("!", nativeNot);
    vm.registerNative("?", nativeIf);
}
