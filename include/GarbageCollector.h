#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "TreeNode.h"
#define FLAG_GC_MARKD 1
#define FLAG_ALLOCATED 2

void markNode(Node *n);
void markAll(void);

#endif
