
layout(binding = 1)uniform TextureArray
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
layout(location=5) out vec4 OutPointLightDepth;

layout(binding = 0)uniform sampler2D InShadowmap;
layout(binding = 1)uniform samplerCube InCubeShadowmap;

in vec2 UV;
in vec3 WorldPosition;
in vec3 Normal;
in vec4 FragFromLight;
flat in uint DrawID;


float ComputePointLightShadow(vec3 FragP, vec3 LightP) {
    
    vec3 FragToLight = FragP - LightP;
    float ClosestDepth = texture(InCubeShadowmap, FragToLight).r;
    
    ClosestDepth *= 10.0;
    
    float CurrentDepth = length(FragToLight);
    
    float Bias = 0.05;
    float Shadow = (CurrentDepth - Bias) > ClosestDepth ? 1.0 : 0.0;
    
    return Shadow;
}

vec4 VisualizePointLightDepth(vec3 FragP, vec3 LightP) {
    
    vec3 FragToLight = FragP - LightP;
    float ClosestDepth = texture(InCubeShadowmap, FragToLight).r;
    
    ClosestDepth *= 10.0;
    vec3 FragColor = vec3(ClosestDepth / 10.0f);
    
    return vec4(FragColor, 1);
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
    
    vec4 FragColor = texture(AlbedoSampler, UV);
    if(0 != (DrawUniform.ShaderFlags & SHADER_FLAG_USE_COLOR)) {
        FragColor = DrawUniform.Color;
    }
    
    //vec4 FragNormal = vec4(ComputeWorldNormal(texture(NormalSampler, UV).xyz), 1);
    vec4 FragNormal = vec4(ComputeWorldNormal(vec3(0.5, 0.5, 1)), 1);
    
    vec4 FragRoughness = vec4(texture(RoughnessSampler, UV).xyz, 1);
    if(0 != (DrawUniform.ShaderFlags & SHADER_FLAG_USE_ROUGHNESS)) {
        FragRoughness.g = DrawUniform.RoughnessFactor;
    }
    if(0 != (DrawUniform.ShaderFlags & SHADER_FLAG_USE_METALLIC)) {
        FragRoughness.b = DrawUniform.MetallicFactor;
    }
    
    float Shadow = ComputePointLightShadow(WorldPosition, vec3(0, 1, 0));
    vec4 FragShadow = vec4(Shadow, Shadow, Shadow, 1);
    
    vec4 FragPosition = vec4(WorldPosition, 1);
    vec4 FragPointLightDepth = VisualizePointLightDepth(FragPosition.xyz, vec3(0, 1, 0));
    
    OutColor = FragColor;
    OutNormal = FragNormal;
    OutRoughness = FragRoughness;
    OutShadow = FragShadow;
    OutWorldSpace = FragPosition;
    OutPointLightDepth = FragPointLightDepth;
}