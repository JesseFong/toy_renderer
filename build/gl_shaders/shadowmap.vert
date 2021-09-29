
layout(location = 0)in vec3 VtxPosition;
layout(location = 1)in vec2 VtxUV;
layout(location = 2)in vec3 VtxNormal;
layout(location = 3)in uint VtxDrawID;


void main() 
{
    uint DrawID = VtxDrawID;
    mat4 Model = DrawUniformArray[DrawID].ModelProj;
    vec4 Position = LightProj * Model * vec4(VtxPosition, 1.0f);
    gl_Position = Position;
}