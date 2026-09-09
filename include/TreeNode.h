#ifndef TREENODE_H
#define TREENODE_H

typedef enum { LITERAL, VARIABLE, FUNCTION, LIST } NodeType;

typedef struct Node {
    struct Node *left;
    struct Node *right;

    union {
        int literal;
        char *var;
        char *func;
        int listLiteral;
    } data;

    NodeType type;

    unsigned int infoFlags;
    unsigned int errorFlags;
    unsigned int statusFlags;
} Node;

#endif // TREENODE_H
