#include "TreeNode.h"
#include <stdint.h>
#include <string.h>
#define SIZE 1024

typedef Node *(*Func)(Node *left, Node *right);

typedef struct EnvEntry {
    char *key;
    uint_fast8_t isFunc; // 1 if is func 0 if var

    union {
        Node *node;
        Func func;
    } val;

    struct EnvEntry *next; // incase of collisions
} EnvEntry;

EnvEntry *envTable[SIZE];

unsigned long hashString(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % SIZE;
}

void envInsert(EnvEntry *newEntry) {
    int pos = hashString(newEntry->key);
    if (envTable[pos] != NULL) {
        if (strcmp(envTable[pos]->key, newEntry->key)) {
            envTable[pos]->val = newEntry->val;
            return;
        }
        EnvEntry *temp = envTable[pos];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newEntry;
    } else {
        envTable[pos] = newEntry;
    }
}

EnvEntry *getEnvEntry(char *key) {
    int pos = hashString(key);
    if (envTable[pos] == NULL)
        return NULL;
    if (strcmp(envTable[pos]->key, key)) {
        EnvEntry *temp = envTable[pos];
        while (temp != NULL) {
            temp = temp->next;
            if (strcmp(temp->key, key))
                break;
        }
        return temp;
    }
    return envTable[pos];
}
