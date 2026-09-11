#include "Environment.h"
#include "Evaluator.h"
#include "TreeNode.h"
#include <stdlib.h>

void registerNative(char *name, Node *(*funcPtr)(Node *)) {
    EnvEntry *entry = malloc(sizeof(EnvEntry));
    entry->key = name;
    entry->params = NULL;
    entry->isFunc = 1;
    entry->val.func = funcPtr;
    entry->next = NULL;
    envInsert(entry);
}

Node *nativeAdd(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return createLiteral(evaluate(left)->data.literal + evaluate(right)->data.literal);
}

Node *nativeSub(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return createLiteral(evaluate(left)->data.literal - evaluate(right)->data.literal);
}

Node *nativeMul(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return createLiteral(evaluate(left)->data.literal * evaluate(right)->data.literal);
}

Node *nativeDiv(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    return createLiteral(evaluate(left)->data.literal / evaluate(right)->data.literal);
}

Node *nativeLessThan(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = evaluate(left);
    Node *r = evaluate(right);
    return createLiteral(l->data.literal < r->data.literal ? 1 : 0);
}

Node *nativeGreaterThan(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = evaluate(left);
    Node *r = evaluate(right);
    return createLiteral(l->data.literal > r->data.literal ? 1 : 0);
}

Node *nativeEquals(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = evaluate(left);
    Node *r = evaluate(right);
    return createLiteral(l->data.literal == r->data.literal ? 1 : 0);
}

Node *nativeAnd(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = evaluate(left);
    if (l->data.literal == 0) return createLiteral(0);
    Node *r = evaluate(right);
    return createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeOr(Node *args) {
    Node *left = args->left;
    Node *right = args->right->left;
    Node *l = evaluate(left);
    if (l->data.literal != 0) return createLiteral(1);
    Node *r = evaluate(right);
    return createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeNot(Node *args) {
    Node *left = args->left;
    Node *l = evaluate(left);
    return createLiteral(l->data.literal == 0 ? 1 : 0);
}

Node *nativeIf(Node *args) {
    Node *condition = args->left;
    Node *trueBranch = args->right->left;
    Node *falseBranch = args->right->right->left;
    
    Node *cond = evaluate(condition);
    if (cond->data.literal == 1) {
        return evaluate(trueBranch);
    } else {
        return evaluate(falseBranch);
    }
}

void registerAllNatives(void) {
    registerNative("+", nativeAdd);
    registerNative("-", nativeSub);
    registerNative("*", nativeMul);
    registerNative("/", nativeDiv);
    registerNative("if", nativeIf);
    registerNative("<", nativeLessThan);
    registerNative(">", nativeGreaterThan);
    registerNative("==", nativeEquals);
    registerNative("and", nativeAnd);
    registerNative("or", nativeOr);
    registerNative("not", nativeNot);
}
