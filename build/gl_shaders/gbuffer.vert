
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

layout(location = 0)in vec3 VtxPosition;
layout(location = 1)in vec2 VtxUV;
layout(location = 2)in vec3 VtxNormal;
layout(location = 3)in uint VtxDrawID;

uniform mat4 CascadeProjs[4];
uniform float CascadeDepths[4];

out vec2 UV;
out vec3 WorldPosition;
out vec3 Normal;
out vec4 FragFromLight;
out float ClipSpaceZ;
out vec4 LightSpacePos[4];
out uint DrawID;

void main() 
{
    DrawID = VtxDrawID;
    mat4 Model = DrawUniformArray[DrawID].ModelProj;
    vec4 Position = CameraProj * Model * vec4(VtxPosition, 1.0f);
    
    for(int i = 0; i < 4; i++) {
        LightSpacePos[i] = CascadeProjs[i] * vec4(VtxPosition, 1.0f);
    }
    ClipSpaceZ = Position.z;
    
    UV = VtxUV;
    WorldPosition = (Model * vec4(VtxPosition, 1.0)).xyz;
    Normal =  (transpose(inverse(Model)) *vec4(VtxNormal, 0.0)).xyz;
    FragFromLight = LightProj * Model * vec4(VtxPosition, 1.0f);
    
    gl_Position = Position;
}