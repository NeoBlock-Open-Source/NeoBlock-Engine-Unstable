#ifndef DATAMODEL_SERVICE_H
#define DATAMODEL_SERVICE_H

#include "instance.h"
#include "service/players_service.h"
#include "service/lighting_service.h"
#include "service/workspace_service.h"
#include "service/sound_service.h"

typedef struct DataModelService {
    Instance instance;
    
    // Core services
    PlayersService* Players;
    LightingService* Lighting;
    WorkspaceService* Workspace;
    SoundService* Sound;
    
    // Game state
    bool IsLoaded;
    bool IsRunning;
    char PlaceId[64];
    char JobId[64];
    
    // Events
    RBXScriptSignal* Loaded;
    RBXScriptSignal* Close;
} DataModelService;

DataModelService* DataModelServiceNew();
void DataModelServiceDestroy(DataModelService* dataModel);

void DataModelServiceStart(DataModelService* dataModel);
void DataModelServiceStop(DataModelService* dataModel);
PlayersService* DataModelServiceGetPlayers(DataModelService* dataModel);
WorkspaceService* DataModelServiceGetWorkspace(DataModelService* dataModel);
LightingService* DataModelServiceGetLighting(DataModelService* dataModel);

#endif
