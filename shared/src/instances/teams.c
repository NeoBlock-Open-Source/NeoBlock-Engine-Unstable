#include "teams.h"
#include <string.h>
#include <stdlib.h>

Team* TeamNew(const char* name, int teamColor) {
    Team* team = (Team*)malloc(sizeof(Team));
    InstanceInit(&team->instance, "Team");
    
    strncpy(team->Name, name, 64);
    team->TeamColor = teamColor;
    team->AutoAssignable = true;
    team->AutoColorCharacters = true;
    team->Players = NULL;
    team->PlayersCount = 0;
    
    team->PlayerAdded = RBXScriptSignalNew();
    team->PlayerRemoved = RBXScriptSignalNew();
    
    return team;
}

void TeamDestroy(Team* team) {
    RBXScriptSignalDestroy(team->PlayerAdded);
    RBXScriptSignalDestroy(team->PlayerRemoved);
    free(team->Players);
    free(team);
}

void TeamAddPlayer(Team* team, Player* player) {
    if (TeamHasPlayer(team, player)) return;
    
    team->Players = realloc(team->Players, (team->PlayersCount + 1) * sizeof(Player*));
    team->Players[team->PlayersCount++] = player;
    player->Team = team;
    player->TeamColor = team->TeamColor;
    
    // Apply team color to character if enabled
    if (team->AutoColorCharacters && player->Character) {
        for (int i = 0; i < player->Character->instance.ChildrenCount; i++) {
            Instance* child = player->Character->instance.Children[i];
            if (InstanceIsA(child, "BasePart")) {
                ((Part*)child)->Color = Color3FromTeamColor(team->TeamColor);
            }
        }
    }
    
    RBXScriptSignalFire(team->PlayerAdded, 1, player);
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
            RBXScriptSignalFire(team->PlayerRemoved, 1, player);
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

void TeamSetColor(Team* team, int newColor) {
    team->TeamColor = newColor;
    
    // Update all players' team colors
    for (int i = 0; i < team->PlayersCount; i++) {
        team->Players[i]->TeamColor = newColor;
        
        // Update character colors if enabled
        if (team->AutoColorCharacters && team->Players[i]->Character) {
            for (int j = 0; j < team->Players[i]->Character->instance.ChildrenCount; j++) {
                Instance* child = team->Players[i]->Character->instance.Children[j];
                if (InstanceIsA(child, "BasePart")) {
                    ((Part*)child)->Color = Color3FromTeamColor(newColor);
                }
            }
        }
    }
}

void TeamSetAutoAssignable(Team* team, bool autoAssignable) {
    team->AutoAssignable = autoAssignable;
}
