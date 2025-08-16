#include "instance.h"
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 1024
#define MAX_PROPERTIES 512

struct Instance {
    char ClassName[64];
    char Name[64];
    
    Instance* Parent;
    Instance* Children[MAX_CHILDREN];
    int ChildrenCount;
    
    Property Properties[MAX_PROPERTIES];
    int PropertiesCount;
    
    RBXScriptSignal** Signals;
    int SignalsCount;
    
    bool Archivable;
    bool Destroyed;
};

Instance* InstanceNew(const char* className) {
    Instance* inst = malloc(sizeof(Instance));
    strncpy(inst->ClassName, className, 64);
    inst->Parent = NULL;
    inst->ChildrenCount = 0;
    inst->PropertiesCount = 0;
    inst->SignalsCount = 0;
    inst->Archivable = true;
    inst->Destroyed = false;
    return inst;
}

void InstanceDestroy(Instance* inst) {
    if (inst->Destroyed) return;
    
    for (int i = 0; i < inst->SignalsCount; i++) {
        RBXScriptSignalDisconnectAll(inst->Signals[i]);
    }
    
    for (int i = 0; i < inst->ChildrenCount; i++) {
        InstanceDestroy(inst->Children[i]);
    }
    
    inst->Destroyed = true;
    free(inst);
}

void InstanceSetParent(Instance* inst, Instance* parent) {
    if (inst->Parent == parent) return;
    
    if (inst->Parent) {
        for (int i = 0; i < inst->Parent->ChildrenCount; i++) {
            if (inst->Parent->Children[i] == inst) {
                memmove(&inst->Parent->Children[i], 
                       &inst->Parent->Children[i+1],
                       (inst->Parent->ChildrenCount - i - 1) * sizeof(Instance*));
                inst->Parent->ChildrenCount--;
                break;
            }
        }
    }
    
    if (parent && parent->ChildrenCount < MAX_CHILDREN) {
        parent->Children[parent->ChildrenCount++] = inst;
        inst->Parent = parent;
    } else {
        inst->Parent = NULL;
    }
}
