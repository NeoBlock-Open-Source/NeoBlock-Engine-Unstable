#ifndef TEAMS_H
#define TEAMS_H

#include "instance.h"
#include "player.h"

typedef struct Team {
    Instance instance;
    char Name[64];
    int TeamColor;
    bool AutoAssignable;
    bool AutoColorCharacters;
    Player** Players;
    int PlayersCount;
    
    // Events
    RBXScriptSignal* PlayerAdded;
    RBXScriptSignal* PlayerRemoved;
} Team;

Team* TeamNew(const char* name, int teamColor);
void TeamDestroy(Team* team);

void TeamAddPlayer(Team* team, Player* player);
void TeamRemovePlayer(Team* team, Player* player);
bool TeamHasPlayer(Team* team, Player* player);
void TeamSetColor(Team* team, int newColor);
void TeamSetAutoAssignable(Team* team, bool autoAssignable);

#endif
