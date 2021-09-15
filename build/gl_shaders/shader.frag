
layout(std140, row_major, binding=0)uniform FrameUniforms 
{
    mat4 CameraProj;
    mat4 LightProj;
    point_light PointLight[4];
    directional_light SunLight;
    vec3 CameraP;
    uint FrambufferTextureToDisplay;
};

layout(binding=1)uniform TextureArray 
{
    uint64_t Textures[1024]; 
};

layout(std430, row_major) buffer DrawUniforms
{
    draw_uniform DrawUniformArray[];
};

layout(location=0) out vec4 OutColor;
layout(location=1) out vec4 OutNormal;
layout(location=2) out vec4 OutRoughness;
layout(location=3) out vec4 OutShadow;
layout(location=4) out vec4 OutWorldSpace; 

layout(binding = 0)uniform sampler2D InShadowmap;

in vec2 UV;
in vec3 WorldPosition;
in vec3 Normal;
in vec4 FragFromLight;
flat in uint DrawID;

float ComputeShadow(vec4 LightSpacePosition, vec3 Normal, vec3 LightDir) {
    
    vec3 ProjCoords = LightSpacePosition.xyz / LightSpacePosition.w;
    
    ProjCoords = ProjCoords * 0.5 + 0.5;
    
    
    float ClosestDepth = texture(InShadowmap, ProjCoords.xy).r;
    float CurrentDepth = ProjCoords.z;
    
    float Bias = max(0.05 * (1.0 - dot(Normal, LightDir)), 0.005);
    
    float Shadow = (CurrentDepth - Bias) > ClosestDepth ? 1.0 : 0.0;
    
    
    return Shadow;
}

vec3 ComputeWorldNormal(vec3 MapNormal) {
    
    MapNormal = MapNormal * 2.0f - 1.0f;
    
    vec3 Q1  = dFdx(WorldPosition);
    vec3 Q2  = dFdy(WorldPosition);
    vec2 st1 = dFdx(UV);
    vec2 st2 = dFdy(UV);
    
    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    
    return normalize(TBN * MapNormal);
}


void main()
{
    draw_uniform DrawUniform = DrawUniformArray[DrawID];
    
    sampler2D AlbedoSampler = sampler2D(Textures[DrawUniform.AlbedoID]);
    sampler2D NormalSampler = sampler2D(Textures[DrawUniform.NormalID]);
    sampler2D RoughnessSampler = sampler2D(Textures[DrawUniform.RoughnessID]);
    
    OutColor = texture(AlbedoSampler, UV);
    vec3 WorldNormal = ComputeWorldNormal(texture(NormalSampler, UV).xyz);
    OutNormal = vec4(WorldNormal, 1.0f);
    OutRoughness = texture(RoughnessSampler, UV);
    
    float Shadow = ComputeShadow(FragFromLight, WorldNormal, -SunLight.Direction); 
    OutShadow = vec4(Shadow, Shadow, Shadow, 1);
    
    if(Shadow > 0.0) {
        //OutColor.rgb -= 0.5;
    }
    
    
    OutWorldSpace = vec4(WorldPosition, 1);
    
    if((DrawUniform.ShaderFlags & SHADER_FLAG_USE_COLOR) == 1) {
        OutColor = DrawUniform.Color;
        
        OutRoughness.r = 0.0;
        OutRoughness.g = DrawUniform.RoughnessFactor;
        OutRoughness.b = DrawUniform.MetallicFactor;
        OutRoughness.a = 1.0;
        
        vec3 GeneratedNormal = ComputeWorldNormal(vec3(0.5, 0.5, 1.0));
        OutNormal = vec4(GeneratedNormal, 1.0);
    }
    
    if((DrawUniform.ShaderFlags & SHADER_FLAG_USE_ROUGHNESS) == 1) {
        OutRoughness.g = DrawUniform.RoughnessFactor;
        OutColor = vec4(0, 1, 0, 1);
    }
    
    if((DrawUniform.ShaderFlags & SHADER_FLAG_USE_METALLIC) == 1) {
        OutColor = vec4(0, 0, 1, 1);
        OutRoughness.b = DrawUniform.MetallicFactor;
        vec3 GeneratedNormal = ComputeWorldNormal(vec3(0.5, 0.5, 1.0));
        OutNormal = vec4(GeneratedNormal, 1.0);
    }
}