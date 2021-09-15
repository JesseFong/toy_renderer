#version 450 core
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require
#extension GL_NV_shader_buffer_load : require

#define PI32 3.14159265359
#define TAU32 6.283185307179

const uint SHADER_FLAG_NONE = 0;
const uint SHADER_FLAG_USE_COLOR = 1 << 0;
const uint SHADER_FLAG_USE_ROUGHNESS = 1 << 1;
const uint SHADER_FLAG_USE_METALLIC = 1 << 2;

struct directional_light {
    vec3 Direction;
    vec3 Color;
};

struct point_light {
    vec3 Position;
    vec3 Color;
    float Constant;
    float Linear;
    float Quadratic;
};

struct draw_uniform {
    uint AlbedoID;
    uint NormalID;
    uint RoughnessID;
    uint OcclusionID;
    
    float RoughnessFactor;
    float MetallicFactor;
    uint ShaderFlags;
    
    vec4 Color;
    
    mat4 ModelProj;
};

