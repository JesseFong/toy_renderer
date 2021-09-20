
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


layout(location=0) out vec4 OutC; //Color
layout(location=1) out vec4 OutN; //Normal
layout(location=2) out vec4 OutP; //WorldPosition
layout(location=3) out vec4 OutRME; //Rough, Metallic, Emission
layout(location=4) out vec4 OutShadow; //Shadow, Point Light Depth

layout(binding = 0)uniform sampler2D InShadowmap;
layout(binding = 1)uniform samplerCubeArray InCubeShadowmap;

in vec2 UV;
in vec3 WorldPosition;
in vec3 Normal;
in vec4 FragFromLight;
flat in uint DrawID;


float ComputePointLightShadow(vec3 FragP, vec3 LightP, float r, int LightIndex) {
    
    vec3 FragToLight = FragP - LightP;
    
    float ClosestDepth = texture(InCubeShadowmap, vec4(FragToLight, LightIndex), 0).r;
    //float ClosestDepth = texture(InCubeShadowmap, FragToLight).r;
    
    ClosestDepth *= r;
    
    float CurrentDepth = length(FragToLight);
    float Bias = 0.05;
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
    
    vec4 FragColor = texture(AlbedoSampler, UV);
    if(0 != (DrawUniform.ShaderFlags & SHADER_FLAG_USE_COLOR)) {
        FragColor = DrawUniform.Color;
    }
    
    if(FragColor.a < 0.5) {
        discard;
    }
    
    vec3 FragNormal = ComputeWorldNormal(texture(NormalSampler, UV).xyz);
    //vec3 FragNormal = ComputeWorldNormal(vec3(0.5, 0.5, 1));
    
    vec3 MaterialSample = texture(RoughnessSampler, UV).xyz;
    float FragRoughness = MaterialSample.g;
    float FragMetallic = MaterialSample.b;
    float FragEmission = 0;
    
    if(0 != (DrawUniform.ShaderFlags & SHADER_FLAG_USE_ROUGHNESS)) {
        FragRoughness = DrawUniform.RoughnessFactor;
    }
    if(0 != (DrawUniform.ShaderFlags & SHADER_FLAG_USE_METALLIC)) {
        FragMetallic = DrawUniform.MetallicFactor;
    }
    
    float FragShadowed = 0;
    for(int i = 0;
        i < 3;
        i++) {
        
        FragShadowed += ComputePointLightShadow(WorldPosition, PointLight[i].Position, PointLight[i].Radius, i);
    }
    FragShadowed /= 3;
    
    vec4 FragPosition = vec4(WorldPosition, 1);
    
    OutC = FragColor;
    OutN = vec4(FragNormal, 1);
    OutP = FragPosition;
    
    vec4 MaterialCombined = vec4(FragRoughness, FragMetallic, FragEmission, 1);
    OutRME = MaterialCombined;
    
    OutShadow = vec4(FragShadowed, 1, 1, 1);
}