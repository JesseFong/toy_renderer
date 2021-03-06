
layout(location=0) out vec4 OutC; //Color
layout(location=1) out vec4 OutN; //Normal
layout(location=2) out vec4 OutP; //WorldPosition
layout(location=3) out vec4 OutRME; //Rough, Metallic, Emission
layout(location=4) out vec4 OutShadow; //Shadow, Point Light Depth

layout(binding = 0)uniform sampler2D InShadowmap;
layout(binding = 1)uniform samplerCubeArray InCubeShadowmap;
layout(binding = 2)uniform sampler2DArray InCascadeShadowmap;

uniform mat4 CascadeProjs[4];
uniform float CascadeDepths[4];

in vec2 UV;
in vec3 WorldPosition;
in vec3 Normal;
in vec4 FragFromLight;
in float ClipSpaceZ;
in vec4 LightSpacePos[4];
flat in uint DrawID;

float ComputeCascadeShadow(int CascadeIndex , vec4 LightSpacePosition) {
    
    vec3 ProjCoords = LightSpacePosition.xyz / LightSpacePosition.w;
    
    vec2 UV;
    UV.x = 0.5 * ProjCoords.x + 0.5;
    UV.y = 0.5 * ProjCoords.y + 0.5;
    
    float CurrentDepth = 0.5 * ProjCoords.z + 0.5;
    float ClosestDepth = texture(InCascadeShadowmap, vec3(UV, CascadeIndex)).r;
    
    float Bias = 0.05;
    float Shadow = (CurrentDepth - Bias) > ClosestDepth ? 1.0 : 0.0;
    
    return Shadow;
}


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
    
    float CascadeShadow = 0;
    for(int i = 0; i < 4; i++) {
        if(ClipSpaceZ <= CascadeDepths[i]) {
            CascadeShadow = ComputeCascadeShadow(i, LightSpacePos[i]);
            
#if 0
            if(i == 0) {
                FragColor *= vec4(1, 0, 0, 1);
            } else if(i == 1) {
                FragColor *= vec4(0, 1, 0, 1);
            } else if(i == 2) {
                FragColor *= vec4(0, 0, 1, 1);
            } else if(i == 3) {
                FragColor *= vec4(1, 0, 1, 1);
            }
#endif
            break;
        }
    }
    
    vec4 FragPosition = vec4(WorldPosition, 1);
    
    OutC = FragColor;
    OutN = vec4(FragNormal, 1);
    OutP = FragPosition;
    
    vec4 MaterialCombined = vec4(FragRoughness, FragMetallic, FragEmission, 1);
    OutRME = MaterialCombined;
    
    OutShadow = vec4(FragShadowed, CascadeShadow, 1, 1);
}