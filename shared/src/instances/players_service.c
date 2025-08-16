#include "players_service.h"
#include <string.h>
#include <stdlib.h>

PlayersService* PlayersServiceNew() {
    PlayersService* service = (PlayersService*)malloc(sizeof(PlayersService));
    InstanceInit(&service->instance, "Players");
    
    service->Players = NULL;
    service->PlayersCount = 0;
    service->MaxPlayers = 16;
    service->RespawnTime = 5.0f;
    service->AutoCharacterCleanup = true;
    strncpy(service->CharacterAppearance, "", 256);
    strncpy(service->CharacterTemplate, "", 256);
    
    return service;
}

void PlayersServiceDestroy(PlayersService* service) {
    for (int i = 0; i < service->PlayersCount; i++) {
        PlayerDestroy(service->Players[i]);
    }
    free(service->Players);
    free(service);
}

Player* PlayersServiceCreatePlayer(PlayersService* service, const char* name, int userId) {
    if (service->PlayersCount >= service->MaxPlayers) {
        return NULL;
    }
    
    Player* player = PlayerNew(name, userId);
    service->Players = realloc(service->Players, (service->PlayersCount + 1) * sizeof(Player*));
    service->Players[service->PlayersCount++] = player;
    InstanceSetParent((Instance*)player, (Instance*)service);
    
    return player;
}

void PlayersServiceRemovePlayer(PlayersService* service, Player* player) {
    for (int i = 0; i < service->PlayersCount; i++) {
        if (service->Players[i] == player) {
            PlayerDestroy(player);
            memmove(&service->Players[i], 
                   &service->Players[i+1],
                   (service->PlayersCount - i - 1) * sizeof(Player*));
            service->PlayersCount--;
            break;
        }
    }
}

Player* PlayersServiceGetPlayerByUserId(PlayersService* service, int userId) {
    for (int i = 0; i < service->PlayersCount; i++) {
        if (service->Players[i]->UserId == userId) {
            return service->Players[i];
        }
    }
    return NULL;
}

Player* PlayersServiceGetPlayerByName(PlayersService* service, const char* name) {
    for (int i = 0; i < service->PlayersCount; i++) {
        if (strcmp(service->Players[i]->Name, name) == 0) {
            return service->Players[i];
        }
    }
    return NULL;
}

void PlayersServiceSetMaxPlayers(PlayersService* service, int maxPlayers) {
    service->MaxPlayers = maxPlayers;
    if (service->PlayersCount > maxPlayers) {
        // Kick excess players (implementation would depend on networking)
    }
}

void PlayersServiceSetRespawnTime(PlayersService* service, float respawnTime) {
    service->RespawnTime = respawnTime > 0 ? respawnTime : 0;
}
