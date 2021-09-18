
layout(std430, row_major) buffer DrawUniforms
{
    draw_uniform DrawUniformArray[];
};

layout(location = 0)in vec3 VtxPosition;
layout(location = 1)in vec2 VtxUV;
layout(location = 2)in vec3 VtxNormal;
layout(location = 3)in uint VtxDrawID;

void main()
{
    mat4 Model = DrawUniformArray[VtxDrawID].ModelProj;
    gl_Position = Model * vec4(VtxPosition, 1.0);
}