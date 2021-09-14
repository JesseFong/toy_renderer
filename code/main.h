/* date = September 4th 2021 0:13 pm */

#ifndef MAIN_H
#define MAIN_H


struct scene {
    b32 IsInitialized;
    gltf_model LoadedModel;
    
#define MAX_GENERATED_MESH_COUNT 32
    generated_mesh GeneratedMeshArray[MAX_GENERATED_MESH_COUNT];
    u32 GeneratedMeshCount;
    u32 GeneratedMeshID;
    
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
