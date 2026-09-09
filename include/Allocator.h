#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "TreeNode.h"

void initAllocator(void);
void pushFreeList(Node *deadNode);
Node *popFreeList(void);
Node *allocNode(void);

#endif
