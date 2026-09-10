#include "Environment.h"
#include "TreeNode.h"
#include <stdlib.h>

#define FLAG_GC_MARKD 1

void markNode(Node *n) {
    if (n == NULL)
        return;
    if (n->statusFlags & FLAG_GC_MARKD)
        return;

    n->statusFlags |= FLAG_GC_MARKD;
    if (n->type == LITERAL)
        return;

    markNode(n->left);
    markNode(n->right);
}

void markAll(void) {
    for (int i = 0; i < ENV_SIZE; i++) {
        EnvEntry *temp = envTable[i];

        while (temp != NULL) {
            if (temp->isFunc == 0 && temp->val.node != NULL) {
                markNode(temp->val.node);
            }
            temp = temp->next;
        }
    }
}
