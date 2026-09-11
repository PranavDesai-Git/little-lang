#include "Evaluator.h"
#include "TreeNode.h"

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

Node *nativeIf(Node *condition, Node *branches) {
    Node *cond = evaluate(condition);
    if (cond->data.literal == 1) {
        return evaluate(branches->left);
    } else {
        return evaluate(branches->right);
    }
}
