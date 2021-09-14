
const uint SHADER_FLAG_NONE = 0;
const uint SHADER_FLAG_USE_COLOR = 1 << 0;
const uint SHADER_FLAG_USE_ROUGHNESS = 1 << 1;
const uint SHADER_FLAG_USE_METALLIC = 1 << 2;

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

layout(binding=1)uniform TextureArray 
{
    uint64_t Textures[1024]; 
};

layout(std430, row_major) buffer DrawUniforms
{
    draw_uniform DrawUniformArray[];
};
