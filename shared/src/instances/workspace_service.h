#ifndef WORKSPACE_SERVICE_H
#define WORKSPACE_SERVICE_H

#include "instance.h"
#include "../part.h"

typedef struct WorkspaceService {
    Instance instance;
    
    // Physics properties
    float Gravity;
    bool AllowThirdPartySales;
    bool StreamingEnabled;
    float StreamingMinRadius;
    float StreamingTargetRadius;
    
    // World properties
    Part* Terrain;
    bool DistributedGameTime;
    float FilteringEnabled;
    
    // Events
    RBXScriptSignal* ChildAdded;
    RBXScriptSignal* ChildRemoved;
} WorkspaceService;

WorkspaceService* WorkspaceServiceNew();
void WorkspaceServiceDestroy(WorkspaceService* workspace);

Part* WorkspaceServiceFindPartOnRay(WorkspaceService* workspace, Ray ray, Instance* ignore);
Instance* WorkspaceServiceFindFirstChild(WorkspaceService* workspace, const char* name, bool recursive);
void WorkspaceServiceSetGravity(WorkspaceService* workspace, float gravity);

#endif
