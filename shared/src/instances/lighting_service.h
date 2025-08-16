#ifndef LIGHTING_SERVICE_H
#define LIGHTING_SERVICE_H

#include "instance.h"

typedef struct LightingService {
    Instance instance;
    
    // Ambient lighting
    Color3 Ambient;
    Color3 OutdoorAmbient;
    float AmbientIntensity;
    
    // Shadows
    bool GlobalShadows;
    float ShadowSoftness;
    
    // Time of day
    float TimeOfDay;
    float SunRotation;
    float MoonRotation;
    
    // Skybox
    char SkyboxBk[256];
    char SkyboxDn[256];
    char SkyboxFt[256];
    char SkyboxLf[256];
    char SkyboxRt[256];
    char SkyboxUp[256];
    
    // Effects
    bool BloomEnabled;
    float BloomIntensity;
    float BloomThreshold;
    bool ColorCorrectionEnabled;
    float Brightness;
    float Contrast;
    float Saturation;
    float ExposureCompensation;
    
    // Fog
    bool FogEnabled;
    float FogStart;
    float FogEnd;
    Color3 FogColor;
    
    // Events
    RBXScriptSignal* Changed;
} LightingService;

LightingService* LightingServiceNew();
void LightingServiceDestroy(LightingService* lighting);

void LightingServiceSetTimeOfDay(LightingService* lighting, float time);
void LightingServiceSetSkybox(LightingService* lighting, 
                            const char* bk, const char* dn, const char* ft,
                            const char* lf, const char* rt, const char* up);
void LightingServiceUpdate(LightingService* lighting, float deltaTime);

#endif
