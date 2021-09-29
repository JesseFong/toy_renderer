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
    float Radius;
    vec3 Color;
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

struct gbuffer_result {
    
    vec3 Albedo;
    vec3 Normal;
    vec3 Position;
    float Roughness;
    float Metallic;
    float Emission;
    float Shadowed;
    float SunShadow;
};

layout(std140, row_major, binding=0)uniform FrameUniforms 
{
    mat4 CameraProj;
    mat4 LightProj;
    point_light PointLight[4];
    directional_light SunLight;
    vec3 CameraP;
    uint FrambufferTextureToDisplay;
};

layout(binding = 1)uniform TextureArray
{
    uint64_t Textures[1024];
};

layout(std430, row_major) buffer DrawUniforms
{
    draw_uniform DrawUniformArray[];
};