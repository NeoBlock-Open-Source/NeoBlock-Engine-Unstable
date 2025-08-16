#ifndef PLAYER_H
#define PLAYER_H

#include "instance.h"
#include "humanoid.h"

typedef struct Player {
    Instance instance;
    char Name[64];
    int UserId;
    int TeamColor;
    float AccountAge;
    Character* Character;
    bool CharacterAutoLoads;
    Player* Parent;
    Humanoid* Humanoid;
    Vector3 RespawnLocation;
    bool CanLoadCharacter;
    bool DataReady;
    int MembershipType;
    int NeutralDamage;
    int TeamDamage;
    int InternalId;
} Player;

Player* PlayerNew(const char* name, int userId);
void PlayerDestroy(Player* player);

void PlayerLoadCharacter(Player* player);
void PlayerRespawn(Player* player);
void PlayerSetTeam(Player* player, Team* team);
void PlayerKick(Player* player, const char* message);
void PlayerChat(Player* player, const char* message);

Character* PlayerGetCharacter(Player* player);
Humanoid* PlayerGetHumanoid(Player* player);
Vector3 PlayerGetRespawnLocation(Player* player);
void PlayerSetRespawnLocation(Player* player, Vector3 location);

#endif
