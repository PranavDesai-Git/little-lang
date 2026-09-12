#ifndef NATIVEFUNCS_H
#define NATIVEFUNCS_H
#include "PluginAPI.h"
#include "TreeNode.h"

void registerNative(char *name, Node *(*funcPtr)(Node *));
void registerAllNatives(void);
void loadPlugin(const char *path, VMAPI api);
#endif
