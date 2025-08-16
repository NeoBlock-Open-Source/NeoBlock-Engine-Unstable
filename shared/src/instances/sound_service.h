#ifndef SOUND_SERVICE_H
#define SOUND_SERVICE_H

#include "instance.h"

typedef struct SoundService {
    Instance instance;
    
    // Global sound properties
    float Volume;
    bool RespectFilteringEnabled;
    float RolloffScale;
    float DopplerScale;
    float DistanceFactor;
    
    // Ambience
    char AmbientReverb[64];
    float AmbientReverbGain;
    
    // Events
    RBXScriptSignal* SoundPlayed;
} SoundService;

SoundService* SoundServiceNew();
void SoundServiceDestroy(SoundService* soundService);

void SoundServicePlay(SoundService* service, const char* soundId, Instance* emitter);
void SoundServiceStop(SoundService* service, const char* soundId);
void SoundServiceSetVolume(SoundService* service, float volume);

#endif
