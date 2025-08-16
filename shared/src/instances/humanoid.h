#ifndef HUMANOID_H
#define HUMANOID_H

#include "instance.h"
#include "part.h"

typedef enum {
    HUMANSTATE_RUNNING,
    HUMANSTATE_JUMPING,
    HUMANSTATE_CLIMBING,
    HUMANSTATE_STRAFING,
    HUMANSTATE_DEAD,
    HUMANSTATE_FREE_FALL,
    HUMANSTATE_FALLING_DOWN,
    HUMANSTATE_SEATED,
    HUMANSTATE_PLATFORM_STANDING,
    HUMANSTATE_SWIMMING,
    HUMANSTATE_PHYSICS
} HumanoidStateType;

typedef enum {
    HUMANOID_DISPLAY_DISTANCE_NONE,
    HUMANOID_DISPLAY_DISTANCE_VIEWER,
    HUMANOID_DISPLAY_DISTANCE_SUBJECT
} HumanoidDisplayDistanceType;

typedef struct Humanoid {
    Instance instance;
    
    // Core properties
    float Health;
    float MaxHealth;
    float WalkSpeed;
    float JumpPower;
    float HipHeight;
    bool AutoJumpEnabled;
    bool AutoRotate;
    HumanoidStateType State;
    HumanoidDisplayDistanceType DisplayDistanceType;
    
    // Body parts
    Part* Torso;
    Part* Head;
    Part* LeftArm;
    Part* RightArm;
    Part* LeftLeg;
    Part* RightLeg;
    
    // Movement states
    bool Sit;
    bool Jump;
    bool PlatformStand;
    bool RequiresNeck;
    
    // Physics
    Vector3 MoveDirection;
    Vector3 RootPartOffset;
    
    // Events
    RBXScriptSignal* Died;
    RBXScriptSignal* StateChanged;
    RBXScriptSignal* HealthChanged;
    RBXScriptSignal* Jumping;
    RBXScriptSignal* Seated;
    RBXScriptSignal* FreeFalling;
    RBXScriptSignal* PlatformStanding;
} Humanoid;

Humanoid* HumanoidNew();
void HumanoidDestroy(Humanoid* humanoid);

void HumanoidTakeDamage(Humanoid* humanoid, float damage);
void HumanoidMove(Humanoid* humanoid, Vector3 direction, bool relativeToCamera);
void HumanoidJump(Humanoid* humanoid);
void HumanoidSit(Humanoid* humanoid);
void HumanoidChangeState(Humanoid* humanoid, HumanoidStateType newState);
void HumanoidUpdate(Humanoid* humanoid, float deltaTime);

#endif
