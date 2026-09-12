#include "PluginAPI.h"

static VMAPI vm;

Node *nativeAdd(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return vm.createLiteral(vm.evaluate(left)->data.literal +
                            vm.evaluate(right)->data.literal);
}

Node *nativeSub(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return vm.createLiteral(vm.evaluate(left)->data.literal -
                            vm.evaluate(right)->data.literal);
}

Node *nativeMul(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return vm.createLiteral(vm.evaluate(left)->data.literal *
                            vm.evaluate(right)->data.literal);
}

Node *nativeDiv(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return vm.createLiteral(vm.evaluate(left)->data.literal /
                            vm.evaluate(right)->data.literal);
}

Node *nativeLessThan(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left);
    Node *r = vm.evaluate(right);
    return vm.createLiteral(l->data.literal < r->data.literal ? 1 : 0);
}

Node *nativeGreaterThan(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left);
    Node *r = vm.evaluate(right);
    return vm.createLiteral(l->data.literal > r->data.literal ? 1 : 0);
}

Node *nativeEquals(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left);
    Node *r = vm.evaluate(right);
    return vm.createLiteral(l->data.literal == r->data.literal ? 1 : 0);
}

Node *nativeAnd(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left);
    if (l->data.literal == 0)
        return vm.createLiteral(0);
    Node *r = vm.evaluate(right);
    return vm.createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeOr(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = vm.evaluate(left);
    if (l->data.literal != 0)
        return vm.createLiteral(1);
    Node *r = vm.evaluate(right);
    return vm.createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeNot(Node *args) {
    Node *left = args->left;
    Node *l = vm.evaluate(left);
    return vm.createLiteral(l->data.literal == 0 ? 1 : 0);
}

Node *nativeIf(Node *args) {
    Node *condition = args->left;
    Node *trueBranch = args->right->left;
    Node *falseBranch = args->right->right->left;

    Node *cond = vm.evaluate(condition);
    if (cond->data.literal == 1) {
        return vm.evaluate(trueBranch);
    } else {
        return vm.evaluate(falseBranch);
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
