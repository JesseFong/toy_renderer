/* date = September 4th 2021 0:13 pm */

#ifndef MAIN_H
#define MAIN_H


struct directional_light {
    v3 Direction;
    float Pad;
    v3 Color;
    float Pad1;
};

struct point_light {
    v3 Position;
    float Radius;
    v3 Color;
    float Pad0;
};

struct renderer_settings {
    u32 BufferToDisplay;
    b32 DisplayGbuffer;
    s32 LightIndex;
    u32 LightCount;
};

struct scene {
    b32 IsInitialized;
    gltf_model LoadedModel;
    
    point_light PointLights[4];
    directional_light SunLight;
    
    u32 SkyboxID;
    
    u32 GeneratedSphereID;
    u32 GeneratedCubeID;
    
    camera Camera;
    
    u32* RenderMeshIDArray;
    u32 RenderMeshIDCount;
    
    u32** RenderMeshTextureArray;
};

struct app_memory {
    memory_arena MainArena;
    memory_arena TempArena;
    scene Scene;
    
    ui_state* UI;
    renderer_settings Settings;
};

#endif //MAIN_H
