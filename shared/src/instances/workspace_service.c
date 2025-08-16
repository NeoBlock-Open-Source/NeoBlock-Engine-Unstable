#include "workspace_service.h"
#include <stdlib.h>

WorkspaceService* WorkspaceServiceNew() {
    WorkspaceService* workspace = (WorkspaceService*)malloc(sizeof(WorkspaceService));
    InstanceInit(&workspace->instance, "Workspace");
    
    workspace->Gravity = 196.2f;
    workspace->AllowThirdPartySales = false;
    workspace->StreamingEnabled = false;
    workspace->StreamingMinRadius = 100.0f;
    workspace->StreamingTargetRadius = 200.0f;
    workspace->Terrain = NULL;
    workspace->DistributedGameTime = false;
    workspace->FilteringEnabled = true;
    
    workspace->ChildAdded = RBXScriptSignalNew();
    workspace->ChildRemoved = RBXScriptSignalNew();
    
    return workspace;
}

void WorkspaceServiceDestroy(WorkspaceService* workspace) {
    if (workspace->Terrain) {
        InstanceDestroy((Instance*)workspace->Terrain);
    }
    RBXScriptSignalDestroy(workspace->ChildAdded);
    RBXScriptSignalDestroy(workspace->ChildRemoved);
    free(workspace);
}

Part* WorkspaceServiceFindPartOnRay(WorkspaceService* workspace, Ray ray, Instance* ignore) {
    // In a real implementation, this would perform raycasting
    // against all parts in the workspace
    
    return NULL;
}

Instance* WorkspaceServiceFindFirstChild(WorkspaceService* workspace, const char* name, bool recursive) {
    for (int i = 0; i < workspace->instance.ChildrenCount; i++) {
        Instance* child = workspace->instance.Children[i];
        if (strcmp(child->Name, name) == 0) {
            return child;
        }
        
        if (recursive) {
            Instance* found = InstanceFindFirstChild(child, name, true);
            if (found) {
                return found;
            }
        }
    }
    return NULL;
}

void WorkspaceServiceSetGravity(WorkspaceService* workspace, float gravity) {
    workspace->Gravity = gravity;
}
