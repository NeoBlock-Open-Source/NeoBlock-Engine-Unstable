#include "sound_service.h"
#include <string.h>
#include <stdlib.h>

SoundService* SoundServiceNew() {
    SoundService* service = (SoundService*)malloc(sizeof(SoundService));
    InstanceInit(&service->instance, "SoundService");
    
    service->Volume = 0.5f;
    service->RespectFilteringEnabled = true;
    service->RolloffScale = 1.0f;
    service->DopplerScale = 1.0f;
    service->DistanceFactor = 3.0f;
    
    strncpy(service->AmbientReverb, "Generic", 64);
    service->AmbientReverbGain = 0.5f;
    
    service->SoundPlayed = RBXScriptSignalNew();
    
    return service;
}

void SoundServiceDestroy(SoundService* service) {
    RBXScriptSignalDestroy(service->SoundPlayed);
    free(service);
}

void SoundServicePlay(SoundService* service, const char* soundId, Instance* emitter) {
    // In a real implementation, this would play the sound
    // through the audio system
    
    RBXScriptSignalFire(service->SoundPlayed, 2, soundId, emitter);
}

void SoundServiceStop(SoundService* service, const char* soundId) {
    // In a real implementation, this would stop the sound
}

void SoundServiceSetVolume(SoundService* service, float volume) {
    service->Volume = volume;
    if (service->Volume < 0.0f) service->Volume = 0.0f;
    if (service->Volume > 1.0f) service->Volume = 1.0f;
    
    // In a real implementation, this would update all active sounds
}
