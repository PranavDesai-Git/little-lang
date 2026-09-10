#include "TreeNode.h"
#include <stdint.h>
#define SIZE 1024

typedef Node *(*Func)(Node *left, Node *right);

typedef struct EnvEntry {
    char *key;
    uint_fast8_t isFunc; // 1 if is func 0 if var

    union {
        int node;
        Func func;
    } val;
} EnvEntry;

unsigned long hashString(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % SIZE;
}
