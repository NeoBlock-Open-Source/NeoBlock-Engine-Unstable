#include "lighting_service.h"
#include <string.h>
#include <stdlib.h>

LightingService* LightingServiceNew() {
    LightingService* lighting = (LightingService*)malloc(sizeof(LightingService));
    InstanceInit(&lighting->instance, "Lighting");
    
    // Default lighting values
    lighting->Ambient = (Color3){0.4f, 0.4f, 0.4f};
    lighting->OutdoorAmbient = (Color3){0.0f, 0.0f, 0.0f};
    lighting->AmbientIntensity = 1.0f;
    
    lighting->GlobalShadows = true;
    lighting->ShadowSoftness = 0.5f;
    
    lighting->TimeOfDay = 12.0f; // Noon
    lighting->SunRotation = 0.0f;
    lighting->MoonRotation = 0.0f;
    
    strncpy(lighting->SkyboxBk, "", 256);
    strncpy(lighting->SkyboxDn, "", 256);
    strncpy(lighting->SkyboxFt, "", 256);
    strncpy(lighting->SkyboxLf, "", 256);
    strncpy(lighting->SkyboxRt, "", 256);
    strncpy(lighting->SkyboxUp, "", 256);
    
    lighting->BloomEnabled = false;
    lighting->BloomIntensity = 0.4f;
    lighting->BloomThreshold = 0.8f;
    lighting->ColorCorrectionEnabled = false;
    lighting->Brightness = 0.0f;
    lighting->Contrast = 0.0f;
    lighting->Saturation = 0.0f;
    lighting->ExposureCompensation = 0.0f;
    
    lighting->FogEnabled = false;
    lighting->FogStart = 0.0f;
    lighting->FogEnd = 100.0f;
    lighting->FogColor = (Color3){0.7f, 0.7f, 0.7f};
    
    lighting->Changed = RBXScriptSignalNew();
    
    return lighting;
}

void LightingServiceDestroy(LightingService* lighting) {
    RBXScriptSignalDestroy(lighting->Changed);
    free(lighting);
}

void LightingServiceSetTimeOfDay(LightingService* lighting, float time) {
    lighting->TimeOfDay = fmodf(time, 24.0f);
    if (lighting->TimeOfDay < 0) lighting->TimeOfDay += 24.0f;
    
    // Update sun/moon rotation based on time
    lighting->SunRotation = lighting->TimeOfDay * 15.0f; // 360/24 = 15 degrees per hour
    lighting->MoonRotation = lighting->SunRotation + 180.0f;
    
    // Update ambient lighting based on time
    if (lighting->TimeOfDay > 5.0f && lighting->TimeOfDay < 19.0f) {
        // Daytime
        lighting->Ambient = (Color3){0.4f, 0.4f, 0.4f};
        lighting->OutdoorAmbient = (Color3){0.8f, 0.8f, 0.8f};
    } else {
        // Nighttime
        lighting->Ambient = (Color3){0.1f, 0.1f, 0.2f};
        lighting->OutdoorAmbient = (Color3){0.0f, 0.0f, 0.0f};
    }
    
    RBXScriptSignalFire(lighting->Changed, 0);
}

void LightingServiceSetSkybox(LightingService* lighting, 
                            const char* bk, const char* dn, const char* ft,
                            const char* lf, const char* rt, const char* up) {
    strncpy(lighting->SkyboxBk, bk, 256);
    strncpy(lighting->SkyboxDn, dn, 256);
    strncpy(lighting->SkyboxFt, ft, 256);
    strncpy(lighting->SkyboxLf, lf, 256);
    strncpy(lighting->SkyboxRt, rt, 256);
    strncpy(lighting->SkyboxUp, up, 256);
    
    RBXScriptSignalFire(lighting->Changed, 0);
}

void LightingServiceUpdate(LightingService* lighting, float deltaTime) {
    // In a real implementation, this would update time progression
    // and any dynamic lighting effects
}
