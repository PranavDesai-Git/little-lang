#include "Environment.h"
#include "Evaluator.h"
#include "TreeNode.h"
#include <stdlib.h>

void registerNative(char *name, Node *(*funcPtr)(Node *, Node *)) {
    EnvEntry *entry = malloc(sizeof(EnvEntry));
    entry->key = name;
    entry->paramName = NULL;
    entry->isFunc = 1;
    entry->val.func = funcPtr;
    entry->next = NULL;
    envInsert(entry);
}

Node *nativeAdd(Node *left, Node *right) {
    return createLiteral(evaluate(left)->data.literal +
                         evaluate(right)->data.literal);
}
Node *nativeSub(Node *left, Node *right) {
    return createLiteral(evaluate(left)->data.literal -
                         evaluate(right)->data.literal);
}
Node *nativeMul(Node *left, Node *right) {
    // TODO: optimize via info flag
    return createLiteral(evaluate(left)->data.literal *
                         evaluate(right)->data.literal);
}
Node *nativeDiv(Node *left, Node *right) {
    // TODO: handle div by zero via fatal flag
    return createLiteral(evaluate(left)->data.literal /
                         evaluate(right)->data.literal);
}

Node *nativeLessThan(Node *left, Node *right) {
    Node *l = evaluate(left);
    Node *r = evaluate(right);
    return createLiteral(l->data.literal < r->data.literal ? 1 : 0);
}

Node *nativeGreaterThan(Node *left, Node *right) {
    Node *l = evaluate(left);
    Node *r = evaluate(right);
    return createLiteral(l->data.literal > r->data.literal ? 1 : 0);
}

Node *nativeEquals(Node *left, Node *right) {
    Node *l = evaluate(left);
    Node *r = evaluate(right);
    return createLiteral(l->data.literal == r->data.literal ? 1 : 0);
}

Node *nativeAnd(Node *left, Node *right) {
    Node *l = evaluate(left);
    if (l->data.literal == 0)
        return createLiteral(0);

    Node *r = evaluate(right);
    return createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeOr(Node *left, Node *right) {
    Node *l = evaluate(left);
    if (l->data.literal != 0)
        return createLiteral(1);

    Node *r = evaluate(right);
    return createLiteral(r->data.literal != 0 ? 1 : 0);
}

Node *nativeNot(Node *left, Node *right) {
    (void)right; // unused
    Node *l = evaluate(left);
    return createLiteral(l->data.literal == 0 ? 1 : 0);
}

Node *nativeIf(Node *condition, Node *branches) {
    Node *cond = evaluate(condition);
    if (cond->data.literal == 1) {
        return evaluate(branches->left);
    } else {
        return evaluate(branches->right);
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
