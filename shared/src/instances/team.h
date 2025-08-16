#ifndef TEAM_H
#define TEAM_H

#include "instance.h"

typedef struct Team {
    Instance instance;
    char Name[64];
    int TeamColor;
    bool AutoAssignable;
    Player** Players;
    int PlayersCount;
} Team;

Team* TeamNew(const char* name, int teamColor);
void TeamDestroy(Team* team);

void TeamAddPlayer(Team* team, Player* player);
void TeamRemovePlayer(Team* team, Player* player);
bool TeamHasPlayer(Team* team, Player* player);

#endif
