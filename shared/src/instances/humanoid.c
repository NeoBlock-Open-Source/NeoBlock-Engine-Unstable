#include "humanoid.h"
#include <math.h>
#include <stdlib.h>

Humanoid* HumanoidNew() {
    Humanoid* humanoid = (Humanoid*)malloc(sizeof(Humanoid));
    InstanceInit(&humanoid->instance, "Humanoid");
    
    // Default values
    humanoid->Health = 100;
    humanoid->MaxHealth = 100;
    humanoid->WalkSpeed = 16;
    humanoid->JumpPower = 50;
    humanoid->HipHeight = 0;
    humanoid->AutoJumpEnabled = true;
    humanoid->AutoRotate = true;
    humanoid->State = HUMANSTATE_RUNNING;
    humanoid->DisplayDistanceType = HUMANOID_DISPLAY_DISTANCE_NONE;
    
    // Body parts
    humanoid->Torso = NULL;
    humanoid->Head = NULL;
    humanoid->LeftArm = NULL;
    humanoid->RightArm = NULL;
    humanoid->LeftLeg = NULL;
    humanoid->RightLeg = NULL;
    
    // States
    humanoid->Sit = false;
    humanoid->Jump = false;
    humanoid->PlatformStand = false;
    humanoid->RequiresNeck = true;
    
    // Physics
    humanoid->MoveDirection = (Vector3){0, 0, 0};
    humanoid->RootPartOffset = (Vector3){0, 0, 0};
    
    // Create signals
    humanoid->Died = RBXScriptSignalNew();
    humanoid->StateChanged = RBXScriptSignalNew();
    humanoid->HealthChanged = RBXScriptSignalNew();
    humanoid->Jumping = RBXScriptSignalNew();
    humanoid->Seated = RBXScriptSignalNew();
    humanoid->FreeFalling = RBXScriptSignalNew();
    humanoid->PlatformStanding = RBXScriptSignalNew();
    
    return humanoid;
}

void HumanoidDestroy(Humanoid* humanoid) {
    RBXScriptSignalDestroy(humanoid->Died);
    RBXScriptSignalDestroy(humanoid->StateChanged);
    RBXScriptSignalDestroy(humanoid->HealthChanged);
    RBXScriptSignalDestroy(humanoid->Jumping);
    RBXScriptSignalDestroy(humanoid->Seated);
    RBXScriptSignalDestroy(humanoid->FreeFalling);
    RBXScriptSignalDestroy(humanoid->PlatformStanding);
    free(humanoid);
}

void HumanoidTakeDamage(Humanoid* humanoid, float damage) {
    if (humanoid->Health <= 0) return;
    
    humanoid->Health -= damage;
    RBXScriptSignalFire(humanoid->HealthChanged, 1, humanoid->Health);
    
    if (humanoid->Health <= 0) {
        humanoid->Health = 0;
        HumanoidChangeState(humanoid, HUMANSTATE_DEAD);
        RBXScriptSignalFire(humanoid->Died, 0);
    }
}

void HumanoidMove(Humanoid* humanoid, Vector3 direction, bool relativeToCamera) {
    if (humanoid->State == HUMANSTATE_DEAD) return;
    
    // Normalize direction
    float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
        direction.z /= length;
    }
    
    humanoid->MoveDirection = direction;
    
    // Apply movement to root part (torso)
    if (humanoid->Torso) {
        Vector3 velocity;
        
        if (relativeToCamera) {
            // Calculate camera-relative movement
            // (Implementation would use camera CFrame)
            velocity.x = direction.x * humanoid->WalkSpeed;
            velocity.y = 0;
            velocity.z = direction.z * humanoid->WalkSpeed;
        } else {
            // World-space movement
            velocity.x = direction.x * humanoid->WalkSpeed;
            velocity.y = 0;
            velocity.z = direction.z * humanoid->WalkSpeed;
        }
        
        // Apply velocity to torso
        humanoid->Torso->Velocity.x = velocity.x;
        humanoid->Torso->Velocity.z = velocity.z;
        
        // Auto-rotate character
        if (humanoid->AutoRotate && (velocity.x != 0 || velocity.z != 0)) {
            float targetAngle = atan2(velocity.x, velocity.z);
            // Smooth rotation would be applied here
        }
    }
}

void HumanoidJump(Humanoid* humanoid) {
    if (humanoid->State == HUMANSTATE_DEAD || 
        humanoid->State == HUMANSTATE_JUMPING || 
        humanoid->State == HUMANSTATE_SWIMMING) {
        return;
    }
    
    if (humanoid->Torso) {
        humanoid->Torso->Velocity.y = humanoid->JumpPower;
        HumanoidChangeState(humanoid, HUMANSTATE_JUMPING);
        RBXScriptSignalFire(humanoid->Jumping, 0);
    }
}

void HumanoidSit(Humanoid* humanoid) {
    humanoid->Sit = !humanoid->Sit;
    
    if (humanoid->Sit) {
        HumanoidChangeState(humanoid, HUMANSTATE_SEATED);
        RBXScriptSignalFire(humanoid->Seated, 1, true);
    } else {
        HumanoidChangeState(humanoid, HUMANSTATE_RUNNING);
        RBXScriptSignalFire(humanoid->Seated, 1, false);
    }
}

void HumanoidChangeState(Humanoid* humanoid, HumanoidStateType newState) {
    if (humanoid->State == newState) return;
    
    HumanoidStateType oldState = humanoid->State;
    humanoid->State = newState;
    
    // Handle state-specific logic
    switch (newState) {
        case HUMANSTATE_DEAD:
            if (humanoid->Torso) {
                humanoid->Torso->Velocity = (Vector3){0, 0, 0};
            }
            break;
            
        case HUMANSTATE_SEATED:
            if (humanoid->Torso) {
                humanoid->Torso->Velocity = (Vector3){0, 0, 0};
            }
            break;
            
        case HUMANSTATE_PLATFORM_STANDING:
            humanoid->PlatformStand = true;
            break;
            
        default:
            humanoid->PlatformStand = false;
            break;
    }
    
    RBXScriptSignalFire(humanoid->StateChanged, 2, oldState, newState);
}

void HumanoidUpdate(Humanoid* humanoid, float deltaTime) {
    if (humanoid->State == HUMANSTATE_DEAD) return;
    
    // Check if humanoid is falling
    if (humanoid->Torso && humanoid->Torso->Velocity.y < -0.1 && 
        humanoid->State != HUMANSTATE_JUMPING && 
        humanoid->State != HUMANSTATE_FREE_FALL) {
        HumanoidChangeState(humanoid, HUMANSTATE_FREE_FALL);
    }
    
    // Update body part positions based on humanoid state
    if (humanoid->Torso && humanoid->Head) {
        // Keep head positioned above torso
        if (humanoid->RequiresNeck) {
            humanoid->Head->Position.x = humanoid->Torso->Position.x;
            humanoid->Head->Position.y = humanoid->Torso->Position.y + 1.5;
            humanoid->Head->Position.z = humanoid->Torso->Position.z;
        }
        
        // Update limb positions based on animation state
        // (Actual implementation would use animation system)
    }
    
    // Apply root part offset for sitting/climbing/etc
    if (humanoid->RootPartOffset.x != 0 || 
        humanoid->RootPartOffset.y != 0 || 
        humanoid->RootPartOffset.z != 0) {
        if (humanoid->Torso) {
            humanoid->Torso->Position.x += humanoid->RootPartOffset.x;
            humanoid->Torso->Position.y += humanoid->RootPartOffset.y;
            humanoid->Torso->Position.z += humanoid->RootPartOffset.z;
        }
    }
}
