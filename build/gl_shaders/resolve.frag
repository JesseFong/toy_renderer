
layout(std140, row_major, binding=0)uniform FrameUniforms 
{
    mat4 CameraProj;
    mat4 LightProj;
    point_light PointLight[4];
    directional_light SunLight;
    vec3 CameraP;
    uint FrambufferTextureToDisplay;
};


layout(binding = 0)uniform sampler2D InGAlbedo;
layout(binding = 1)uniform sampler2D InGNormal;
layout(binding = 2)uniform sampler2D InGRoughness;
layout(binding = 3)uniform sampler2D InShadowed;
layout(binding = 4)uniform sampler2D InGPosition;
layout(binding = 5)uniform sampler2D InPointLightDepth;
layout(binding = 6)uniform sampler2D InDepthBuffer;

layout(location=0) out vec4 OutColor;

in vec2 UV;

float CalculatePointLightAttenuation(vec3 VectorFromLight, float K0, float K1, float K2) {
    
    float Distance = length(VectorFromLight);
    float Attenuation = 1.0 / (K0 + (K1 * Distance) + (K2 * (Distance*Distance)));
    return Attenuation;
}

float BlinnPhongSpecular(vec3 Normal, vec3 LightDir, vec3 FragP, vec3 ViewP, float SpecularExponent) {
    
    vec3 ViewDir = normalize(ViewP - FragP);
    vec3 HalfDir = normalize(LightDir + ViewDir);
    
    float Specular = pow(max(dot(Normal, HalfDir), 0.0), SpecularExponent);
    return Specular;
}

float PhongSpecular(vec3 Normal, vec3 LightDir, vec3 FragP, vec3 ViewP, float SpecularExponent) {
    
    vec3 ViewDir = normalize(ViewP - FragP);
    vec3 ReflectionDir = reflect(-LightDir, Normal);
    float Specular = pow(max(dot(ViewDir, ReflectionDir), 0.0), SpecularExponent);
    return Specular;
}

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

#if 0
vec3 PointLightFunction(vec3 Albedo, point_light Light, vec3 FragPosition, vec3 ViewPosition, vec3 FragNormal, float Roughness, float Metallic) {
    
    vec3 LightPosition = Light.Position;
    vec3 LightColor = Light.Color;
    
    float AmbientFactor = 0.1;
    vec3 Ambient = AmbientFactor * LightColor;
    float DiffuseFactor = max(dot(FragNormal, LightDirection), 0.0);
    vec3 Diffuse = DiffuseFactor * LightColor * Roughness;
    
    float SpecularFactor = 0.5;
    
    //float D = DistributionGGX(FragNormal, HalfVector, FragRoughness);
    //float F = FresnelSchlick(ViewDirection, Normal);
    //float G = GeometrySmith(FragNormal, ViewDirection, LightDirection, FragRoughness);
    
    
    float Spec = BlinnPhongSpecular(FragNormal, LightDirection, FragPosition, ViewP, 32);
    //float Spec = PhongSpecular(FragNormal, Dir, FragPosition, CameraP, 32);
    vec3 Specular = SpecularFactor * Spec * vec3(1, 1, 1) * 1.0;
    
    float Attenuation = CalculatePointLightAttenuation(FragFromLight, Light.Constant, Light.Linear, Light.Quadratic);
    
    Ambient *= Attenuation;
    Diffuse *= Attenuation;
    Specular*= Attenuation;
    
    return Ambient + Diffuse + Specular;
    
}
#endif

vec3 ApplyGamma(vec3 Color, float Gamma) {
    vec3 Result = pow(Color, vec3(1.0/Gamma));
    return Result;
}


void main()
{
    
    vec4 FullAlbedo = texture(InGAlbedo, UV);
    
    if(FullAlbedo.a < 0.5) {
        discard;
    }
    vec3 Albedo = FullAlbedo.rgb;
    vec3 Normal = texture(InGNormal, UV).rgb;
    vec3 RoughnessMetallic = texture(InGRoughness, UV).rgb;
    vec3 FragPosition = texture(InGPosition, UV).rgb;
    float Depth = texture(InDepthBuffer, UV).r;
    float Shadowed = texture(InShadowed, UV).r;
    float DepthVis = texture(InPointLightDepth, UV).r; 
    
    
    vec3 N = normalize(Normal);
    vec3 V = normalize(CameraP  - FragPosition);
    
    float Roughness = RoughnessMetallic.g;
    float Metallic = RoughnessMetallic.b;
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);
    
    vec3 PointLightContribution = vec3(0, 0, 0);
    for(int i = 0; i < 4; i++) {
        
        point_light Light = PointLight[i];
        vec3 LightPosition = Light.Position;
        vec3 LightColor = Light.Color;
        
        vec3 L = normalize(LightPosition - FragPosition);
        vec3 H = normalize(V + L);
        
        float Distance = length(LightPosition - FragPosition);
        float Attenuation = 1.0 / (Distance * Distance);
        vec3 Radiance = LightColor * Attenuation;
        
        float D = DistributionGGX(N, H, clamp(Roughness, 0.0, 1.0));
        float G = GeometrySmith(N, V, L, Roughness);
        vec3 F = FresnelSchlick(H, V, F0);
        
        vec3 KS = F;
        vec3 KD = vec3(1.0) - KS;
        KD *= 1.0 - Metallic;
        
        vec3 Numerator = D*G*F;
        float Denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 Specular = Numerator / Denominator;
        
        float NL = max(dot(N, L), 0.0);
        PointLightContribution += (KD * Albedo / PI32 + Specular) * Radiance * NL;
    }
    
    PointLightContribution = PointLightContribution * (1 - Shadowed);
    
#if 0
    //Sun LightContribution
    if(Shadowed > 0.0) {
        PointLightContribution += vec3(0.0);
    } else {
        directional_light Light = SunLight;
        vec3 LightColor = Light.Color;
        
        vec3 L = normalize(Light.Direction);
        vec3 H = normalize(V + L);
        vec3 Radiance = LightColor;
        
        float D = DistributionGGX(N, H, clamp(Roughness, 0.0, 1.0));
        float G = GeometrySmith(N, V, L, Roughness);
        
        vec3 F = FresnelSchlick(H, V, F0);
        
        vec3 KS = F;
        vec3 KD = vec3(1.0) - KS;
        KD *= 1.0 - Metallic;
        
        vec3 Numerator = D*G*F;
        float Denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        
        vec3 Specular = Numerator / Denominator;
        
        float NL = max(dot(N, L), 0.0);
        PointLightContribution += (KD * Albedo / PI32 + Specular) * Radiance * NL;
    }
#endif
    
    vec3 Ambient = vec3(0.03) * Albedo;
    vec3 FinalColor = Ambient + PointLightContribution;
    FinalColor = ApplyGamma(FinalColor, 2.2);
    OutColor = vec4(vec3(FinalColor), 1);
    
}