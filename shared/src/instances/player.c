#include "player.h"
#include "character.h"
#include "teams.h"
#include <string.h>
#include <stdlib.h>

Player* PlayerNew(const char* name, int userId) {
    Player* player = (Player*)malloc(sizeof(Player));
    InstanceInit(&player->instance, "Player");
    
    strncpy(player->Name, name, 64);
    player->UserId = userId;
    player->Character = NULL;
    player->CharacterAutoLoads = true;
    player->CanLoadCharacter = true;
    player->DataReady = false;
    player->MembershipType = 0; // None
    player->NeutralDamage = 0;
    player->TeamDamage = 0;
    player->Humanoid = NULL;
    player->RespawnLocation = (Vector3){0, 0, 0};
    player->InternalId = rand();
    
    return player;
}

void PlayerDestroy(Player* player) {
    if (player->Character) {
        InstanceDestroy((Instance*)player->Character);
    }
    free(player);
}

void PlayerLoadCharacter(Player* player) {
    if (!player->CanLoadCharacter) return;
    
    // Remove existing character if any
    if (player->Character) {
        InstanceDestroy((Instance*)player->Character);
    }
    
    // Create new character model
    player->Character = CharacterNew();
    strncpy(player->Character->instance.Name, player->Name, 64);
    
    // Set up humanoid
    player->Humanoid = HumanoidNew();
    InstanceSetParent((Instance*)player->Humanoid, (Instance*)player->Character);
    
    // Create R6 body parts
    Part* head = PartNew();
    strncpy(head->instance.Name, "Head", 64);
    head->Size = (Vector3){2, 1, 1};
    head->Position = (Vector3){0, 1.5, 0};
    head->Anchored = false;
    head->CanCollide = true;
    InstanceSetParent((Instance*)head, (Instance*)player->Character);
    
    Part* torso = PartNew();
    strncpy(torso->instance.Name, "Torso", 64);
    torso->Size = (Vector3){2, 2, 1};
    torso->Position = (Vector3){0, 0, 0};
    torso->Anchored = false;
    torso->CanCollide = true;
    InstanceSetParent((Instance*)torso, (Instance*)player->Character);
    
    Part* leftArm = PartNew();
    strncpy(leftArm->instance.Name, "Left Arm", 64);
    leftArm->Size = (Vector3){1, 2, 1};
    leftArm->Position = (Vector3){-1.5, 0, 0};
    leftArm->Anchored = false;
    leftArm->CanCollide = true;
    InstanceSetParent((Instance*)leftArm, (Instance*)player->Character);
    
    Part* rightArm = PartNew();
    strncpy(rightArm->instance.Name, "Right Arm", 64);
    rightArm->Size = (Vector3){1, 2, 1};
    rightArm->Position = (Vector3){1.5, 0, 0};
    rightArm->Anchored = false;
    rightArm->CanCollide = true;
    InstanceSetParent((Instance*)rightArm, (Instance*)player->Character);
    
    Part* leftLeg = PartNew();
    strncpy(leftLeg->instance.Name, "Left Leg", 64);
    leftLeg->Size = (Vector3){1, 2, 1};
    leftLeg->Position = (Vector3){-0.5, -2, 0};
    leftLeg->Anchored = false;
    leftLeg->CanCollide = true;
    InstanceSetParent((Instance*)leftLeg, (Instance*)player->Character);
    
    Part* rightLeg = PartNew();
    strncpy(rightLeg->instance.Name, "Right Leg", 64);
    rightLeg->Size = (Vector3){1, 2, 1};
    rightLeg->Position = (Vector3){0.5, -2, 0};
    rightLeg->Anchored = false;
    rightLeg->CanCollide = true;
    InstanceSetParent((Instance*)rightLeg, (Instance*)player->Character);
    
    // Create welds to connect body parts
    Weld* neck = WeldNew();
    neck->Part0 = torso;
    neck->Part1 = head;
    neck->C0 = CFrame_new(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    neck->C1 = CFrame_new(0, -0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    InstanceSetParent((Instance*)neck, (Instance*)torso);
    
    Weld* leftShoulder = WeldNew();
    leftShoulder->Part0 = torso;
    leftShoulder->Part1 = leftArm;
    leftShoulder->C0 = CFrame_new(-1.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    leftShoulder->C1 = CFrame_new(0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    InstanceSetParent((Instance*)leftShoulder, (Instance*)torso);
    
    Weld* rightShoulder = WeldNew();
    rightShoulder->Part0 = torso;
    rightShoulder->Part1 = rightArm;
    rightShoulder->C0 = CFrame_new(1.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    rightShoulder->C1 = CFrame_new(-0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    InstanceSetParent((Instance*)rightShoulder, (Instance*)torso);
    
    Weld* leftHip = WeldNew();
    leftHip->Part0 = torso;
    leftHip->Part1 = leftLeg;
    leftHip->C0 = CFrame_new(-0.5, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    leftHip->C1 = CFrame_new(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    InstanceSetParent((Instance*)leftHip, (Instance*)torso);
    
    Weld* rightHip = WeldNew();
    rightHip->Part0 = torso;
    rightHip->Part1 = rightLeg;
    rightHip->C0 = CFrame_new(0.5, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    rightHip->C1 = CFrame_new(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    InstanceSetParent((Instance*)rightHip, (Instance*)torso);
    
    // Configure humanoid
    player->Humanoid->MaxHealth = 100;
    player->Humanoid->Health = 100;
    player->Humanoid->WalkSpeed = 16;
    player->Humanoid->JumpPower = 50;
    player->Humanoid->HipHeight = 0;
    player->Humanoid->AutoJumpEnabled = true;
    player->Humanoid->DisplayDistanceType = "None";
    
    // Set humanoid references
    player->Humanoid->Torso = torso;
    player->Humanoid->Head = head;
    player->Humanoid->LeftArm = leftArm;
    player->Humanoid->RightArm = rightArm;
    player->Humanoid->LeftLeg = leftLeg;
    player->Humanoid->RightLeg = rightLeg;
}

void PlayerRespawn(Player* player) {
    if (!player->Character) {
        PlayerLoadCharacter(player);
    }
    
    // Reset character position
    if (player->Character) {
        Model* character = player->Character;
        character->PrimaryPart->Position = player->RespawnLocation;
        
        // Reset humanoid state
        if (player->Humanoid) {
            player->Humanoid->Health = player->Humanoid->MaxHealth;
            player->Humanoid->Sit = false;
            player->Humanoid->PlatformStand = false;
        }
    }
}

void PlayerSetTeam(Player* player, Team* team) {
    if (player->Team == team) return;
    
    // Remove from current team
    if (player->Team) {
        for (int i = 0; i < player->Team->PlayersCount; i++) {
            if (player->Team->Players[i] == player) {
                memmove(&player->Team->Players[i], 
                       &player->Team->Players[i+1],
                       (player->Team->PlayersCount - i - 1) * sizeof(Player*));
                player->Team->PlayersCount--;
                break;
            }
        }
    }
    
    // Add to new team
    if (team) {
        team->Players = realloc(team->Players, (team->PlayersCount + 1) * sizeof(Player*));
        team->Players[team->PlayersCount++] = player;
        player->Team = team;
        player->TeamColor = team->TeamColor;
    } else {
        player->Team = NULL;
        player->TeamColor = 0;
    }
}

void PlayerKick(Player* player, const char* message) {
    // Network implementation would go here
    printf("Kicking player %s: %s\n", player->Name, message);
    
    // Clean up player
    if (player->Character) {
        InstanceDestroy((Instance*)player->Character);
    }
    
    // Remove from players service
    PlayersService* service = (PlayersService*)player->instance.Parent;
    if (service && service->instance.ClassName == "Players") {
        for (int i = 0; i < service->PlayersCount; i++) {
            if (service->Players[i] == player) {
                memmove(&service->Players[i], 
                       &service->Players[i+1],
                       (service->PlayersCount - i - 1) * sizeof(Player*));
                service->PlayersCount--;
                break;
            }
        }
    }
}

Character* PlayerGetCharacter(Player* player) {
    return player->Character;
}

Humanoid* PlayerGetHumanoid(Player* player) {
    return player->Humanoid;
}

Vector3 PlayerGetRespawnLocation(Player* player) {
    return player->RespawnLocation;
}

void PlayerSetRespawnLocation(Player* player, Vector3 location) {
    player->RespawnLocation = location;
}
