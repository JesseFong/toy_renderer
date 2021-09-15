/* date = September 4th 2021 0:13 pm */

#ifndef MAIN_H
#define MAIN_H


struct scene {
    b32 IsInitialized;
    gltf_model LoadedModel;
    
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
};

#endif //MAIN_H
