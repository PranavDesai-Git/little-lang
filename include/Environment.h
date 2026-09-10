#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "TreeNode.h"
#include <stdint.h>

typedef Node *(*Func)(Node *left, Node *right);

typedef struct EnvEntry {
    char *key;
    uint_fast8_t isFunc; // 1 if is func 0 if var

    union {
        Node *node;
        Func func;
    } val;

    struct EnvEntry *next; // in case of collisions
} EnvEntry;

void envInsert(EnvEntry *newEntry);
EnvEntry *getEnvEntry(char *key);

#endif
