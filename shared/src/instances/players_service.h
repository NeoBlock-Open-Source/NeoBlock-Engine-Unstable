#ifndef PLAYERS_SERVICE_H
#define PLAYERS_SERVICE_H

#include "instance.h"
#include "player.h"

typedef struct PlayersService {
    Instance instance;
    Player** Players;
    int PlayersCount;
    int MaxPlayers;
    float RespawnTime;
    bool AutoCharacterCleanup;
    char CharacterAppearance[256];
    char CharacterTemplate[256];
} PlayersService;

PlayersService* PlayersServiceNew();
void PlayersServiceDestroy(PlayersService* service);

Player* PlayersServiceCreatePlayer(PlayersService* service, const char* name, int userId);
void PlayersServiceRemovePlayer(PlayersService* service, Player* player);
Player* PlayersServiceGetPlayerByUserId(PlayersService* service, int userId);
Player* PlayersServiceGetPlayerByName(PlayersService* service, const char* name);

void PlayersServiceSetMaxPlayers(PlayersService* service, int maxPlayers);
void PlayersServiceSetRespawnTime(PlayersService* service, float respawnTime);

#endif
