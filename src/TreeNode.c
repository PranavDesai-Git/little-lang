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
    unsigned int errorFalgs;
    unsigned int statusFlags;
} Node;

/*
Node *createLiteral(int value) {
    Node *n = alloc_node();
    n->type = LITERAL;
    n->data.literal = value;
    return n;
}
*/
