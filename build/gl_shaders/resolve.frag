
layout(std140, row_major, binding=0)uniform FrameUniforms 
{
    mat4 CameraProj;
    mat4 LightProj;
    point_light PointLight[4];
    directional_light SunLight;
    vec3 CameraP;
    uint FrambufferTextureToDisplay;
};


layout(binding = 0)uniform sampler2D InG0;
layout(binding = 1)uniform sampler2D InG1;
layout(binding = 2)uniform sampler2D InG2;
layout(binding = 3)uniform sampler2D InG3;
layout(binding = 4)uniform sampler2D InG4;
layout(binding = 5)uniform sampler2D InGDepth;

layout(location=0) out vec4 OutColor;

in vec2 UV;

float DistributionGGX(vec3 Normal, vec3 HalfVector, float Roughness) {
    
    Roughness = max(Roughness, 0.02);
    float A = Roughness * Roughness;
    float A2 = A*A; 
    float NH = max(dot(Normal, HalfVector), 0.0);
    float NH2 = NH * NH;
    
    float Numerator = A2;
    float Denom = (NH2 * (Numerator - 1.0) + 1.0);
    Denom = PI32 * Denom * Denom;
    
    return Numerator / Denom;
}

float GeometrySchlickGGX(float NormalView, float Roughness) {
    
    float R = (Roughness + 1.0);
    float K = (R*R) / 8.0;
    
    float Numerator = NormalView;
    float Denom = NormalView * (1.0 - K) + K;
    
    return Numerator / Denom;
}

float GeometrySmith(vec3 Normal, vec3 ViewDirection, vec3 LightDirection, float Roughness) {
    
    float NV = max(dot(Normal, ViewDirection), 0.0);
    float NL = max(dot(Normal, LightDirection), 0.0);
    float GGX1 = GeometrySchlickGGX(NV, Roughness);
    float GGX2 = GeometrySchlickGGX(NL, Roughness);
    return GGX1 * GGX2;
}

vec3 FresnelSchlick(vec3 H, vec3 V, vec3 F0) {
    float HV = max(dot(H, V), 0.0);
    //HV = min(HV, 1.0);
    return F0 + (1.0 - F0) * pow(clamp(1.0 - HV, 0.0, 1.0), 5.0);
}

vec3 PBR(vec3 LightDirection, vec3 LightRadiance, vec3 Normal, vec3 ViewDirection, vec3 Albedo, vec3 F0, float Roughness, float Metallic) {
    
    vec3 Half = normalize(ViewDirection + LightDirection);
    float NV = clamp(dot(Normal, ViewDirection), 0, 1);
    float NL = clamp(dot(Normal, LightDirection), 0, 1);
    
    float NDF = DistributionGGX(Normal, Half, clamp(Roughness, 0.0, 1.0));
    float G = GeometrySmith(Normal, ViewDirection, LightDirection, Roughness);
    vec3 F = FresnelSchlick(Half, ViewDirection, F0);
    
    vec3 KS = F;
    vec3 KD = vec3(1.0) - KS;
    KD *= 1.0 - Metallic;
    
    vec3 Numerator = NDF * G * F;
    float Denominator = 4.0 * NV * NL;
    
    vec3 Specular = Numerator / max(Denominator, 0.0001);
    vec3 Diffuse = KD * Albedo / PI32;
    
    vec3 BRDF = Diffuse + Specular;
    
    vec3 Result = BRDF * LightRadiance * NL;
    return Result;
}

vec3 CalculatePointLight(point_light Light, vec3 FragPosition,vec3 Normal, vec3 ViewDirection, vec3 Albedo, vec3 F0, float Roughness, float Metallic) {
    
    vec3 LightDirection = normalize(Light.Position - FragPosition);
    
    float Distance = length(Light.Position - FragPosition);
    float Attenuation = 1.0 / (Distance * Distance);
    vec3 LightRadiance = Light.Color * Attenuation * 10.0;
    
    return PBR(LightDirection, LightRadiance, Normal, ViewDirection, Albedo, F0, Roughness, Metallic);
}

vec3 CalculateDirectionalLight(directional_light Light, vec3 Normal, vec3 ViewDirection, vec3 Albedo, vec3 F0, float Roughness, float Metallic) {
    return PBR(Light.Direction, Light.Color, Normal, ViewDirection, Albedo, F0, Roughness, Metallic);
}

vec3 ApplyGamma(vec3 Color, float Gamma) {
    vec3 Result = pow(Color, vec3(1.0/Gamma));
    return Result;
}

gbuffer_result
ExtractGBufferMaterials() {
    
    gbuffer_result Result;
    
    vec3 G0 = texture(InG0, UV).rgb;
    vec3 G1 = texture(InG1, UV).rgb;
    vec3 G2 = texture(InG2, UV).rgb;
    vec3 G3 = texture(InG3, UV).rgb;
    vec2 G4 = texture(InG4, UV).rg;
    
    Result.Albedo = G0;
    Result.Normal = G1;
    Result.Position = G2;
    Result.Roughness = G3.r;
    Result.Metallic = G3.g;
    Result.Emission = G3.b;
    Result.Shadowed = G4.r;
    Result.SunShadow = G4.g;
    
    return Result;
};


void main()
{
    
    gbuffer_result Frag = ExtractGBufferMaterials();
    
    vec3 N = normalize(Frag.Normal);
    vec3 V = normalize(CameraP  - Frag.Position);
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Frag.Albedo, Frag.Metallic);
    
    vec3 LightContribution = vec3(0, 0, 0);
#if 1
    for(int i = 0; i < 3; i++) {
        point_light Light = PointLight[i];
        
        float Distance = length(Light.Position - Frag.Position);
        if(Distance < Light.Radius) {
            vec3 ThisLightContribution = CalculatePointLight(Light, Frag.Position, N, V, Frag.Albedo, F0, Frag.Roughness, Frag.Metallic);
            ThisLightContribution *= (1 - Frag.Shadowed);
            LightContribution += ThisLightContribution;
        }
    }
#endif
    
#if 1
    vec3 SunLightContribution = CalculateDirectionalLight(SunLight, N, V, Frag.Albedo, F0, Frag.Roughness, Frag.Metallic);
    SunLightContribution *= (1 - Frag.SunShadow);
    LightContribution += SunLightContribution;
    
#endif
    
    vec3 Ambient = vec3(0.03) * Frag.Albedo;
    
    vec3 FinalColor = Ambient + LightContribution;
    FinalColor = ApplyGamma(FinalColor, 2.2);
    OutColor = vec4(vec3(FinalColor), 1);
    
}