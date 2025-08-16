#include "character.h"
#include "humanoid.h"

Character* CharacterNew() {
    Character* character = (Character*)malloc(sizeof(Character));
    ModelInit(&character->model, "Model");
    
    character->Humanoid = NULL;
    character->AutoLoad = true;
    character->Health = 100;
    character->MaxHealth = 100;
    character->RespawnLocation = (Vector3){0, 0, 0};
    
    return character;
}

void CharacterDestroy(Character* character) {
    if (character->Humanoid) {
        InstanceDestroy((Instance*)character->Humanoid);
    }
    ModelDestroy(&character->model);
    free(character);
}

void CharacterRespawn(Character* character) {
    if (character->model.PrimaryPart) {
        character->model.PrimaryPart->Position = character->RespawnLocation;
    }
    
    if (character->Humanoid) {
        character->Humanoid->Health = character->Humanoid->MaxHealth;
        character->Humanoid->State = HUMANSTATE_RUNNING;
    }
}

void CharacterLoadAppearance(Character* character, const char* appearanceId) {
    // For now, the noob skin will be loaded

void CharacterApplyTemplate(Character* character, const char* templateName) {
    // For now let's use a Noob skin. Arms and Legs and Head to yellow and torso to some kind of blue
}
