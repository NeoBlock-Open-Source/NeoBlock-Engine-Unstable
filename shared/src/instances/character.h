#ifndef CHARACTER_H
#define CHARACTER_H

#include "model.h"

typedef struct Character {
    Model model;
    Humanoid* Humanoid;
    bool AutoLoad;
    float Health;
    float MaxHealth;
    Vector3 RespawnLocation;
} Character;

Character* CharacterNew();
void CharacterDestroy(Character* character);

void CharacterRespawn(Character* character);
void CharacterLoadAppearance(Character* character, const char* appearanceId);
void CharacterApplyTemplate(Character* character, const char* templateName);

#endif
