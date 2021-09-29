
layout(location = 0)in vec3 VtxPosition;
layout(location = 3)in uint VtxDrawID;

void main()
{
    mat4 Model = DrawUniformArray[VtxDrawID].ModelProj;
    gl_Position = Model * vec4(VtxPosition, 1.0);
}