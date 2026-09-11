#include "TreeNode.h"
#include "Allocator.h"
#include <stdlib.h>
#include <string.h>

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

Node *copyTree(Node *root) {
    if (root == NULL)
        return NULL;
    Node *clone = allocNode();
    clone->type = root->type;
    clone->data = root->data;

    clone->left = copyTree(root->left);
    clone->right = copyTree(root->right);

    return clone;
}

Node *substitute(Node *root, char *paramName, Node *argValue) {
    if (root == NULL)
        return NULL;

    if (root->type == VARIABLE && strcmp(root->data.var, paramName) == 0) {
        return copyTree(argValue);
    }

    root->left = substitute(root->left, paramName, argValue);
    root->right = substitute(root->right, paramName, argValue);

    return root;
}
