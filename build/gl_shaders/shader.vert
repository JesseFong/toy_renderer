
layout(location = 0)in vec3 VtxPosition;
layout(location = 1)in vec2 VtxUV;
layout(location = 2)in vec3 VtxNormal;
layout(location = 3)in vec4 VtxTangents;
layout(location = 4)in uint VtxDrawID;

out vec2 UV;
out vec3 WorldPosition;
out vec3 Normal;
out vec4 FragFromLight;
out uint DrawID;

void main() 
{
    DrawID = VtxDrawID;
    mat4 Model = DrawUniformArray[DrawID].ModelProj;
    vec4 Position = CameraProj * Model * vec4(VtxPosition, 1.0f);
    gl_Position = Position;
    WorldPosition = (Model * vec4(VtxPosition, 1.0)).xyz;
    FragFromLight = LightProj * Model * vec4(VtxPosition, 1.0f);
    
    UV = VtxUV;
    Normal =  (transpose(inverse(Model)) *vec4(VtxNormal, 0.0)).xyz;
}