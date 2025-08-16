#include "team.h"
#include <string.h>
#include <stdlib.h>

Team* TeamNew(const char* name, int teamColor) {
    Team* team = (Team*)malloc(sizeof(Team));
    InstanceInit(&team->instance, "Team");
    
    strncpy(team->Name, name, 64);
    team->TeamColor = teamColor;
    team->AutoAssignable = true;
    team->Players = NULL;
    team->PlayersCount = 0;
    
    return team;
}

void TeamDestroy(Team* team) {
    free(team->Players);
    free(team);
}

void TeamAddPlayer(Team* team, Player* player) {
    if (TeamHasPlayer(team, player)) return;
    
    team->Players = realloc(team->Players, (team->PlayersCount + 1) * sizeof(Player*));
    team->Players[team->PlayersCount++] = player;
    player->Team = team;
    player->TeamColor = team->TeamColor;
}

void TeamRemovePlayer(Team* team, Player* player) {
    for (int i = 0; i < team->PlayersCount; i++) {
        if (team->Players[i] == player) {
            memmove(&team->Players[i], 
                   &team->Players[i+1],
                   (team->PlayersCount - i - 1) * sizeof(Player*));
            team->PlayersCount--;
            player->Team = NULL;
            player->TeamColor = 0;
            break;
        }
    }
}

bool TeamHasPlayer(Team* team, Player* player) {
    for (int i = 0; i < team->PlayersCount; i++) {
        if (team->Players[i] == player) {
            return true;
        }
    }
    return false;
}
