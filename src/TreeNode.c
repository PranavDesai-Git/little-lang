#include "TreeNode.h"
#include "Allocator.h"

Node *createLiteral(int value) {
    Node *n = allocNode();
    n->type = LITERAL;
    n->data.literal = value;
    return n;
}

Node *createVariable(char *varName) {
    Node *n = allocNode();
    n->type = VARIABLE;
    n->data.var = varName;
    return n;
}

Node *createFunction(char *funcName, Node *leftChild, Node *rightChild) {
    Node *n = allocNode();
    n->type = FUNCTION;
    n->data.func = funcName;
    n->left = leftChild;
    n->right = rightChild;
    return n;
}

Node *createList(int value, Node *nextNode) {
    Node *n = allocNode();
    n->type = LIST;
    n->data.listLiteral = value;
    n->right = nextNode;
    return n;
}
