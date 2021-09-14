
layout(location = 0)in vec3 VtxPosition;
layout(location = 1)in vec2 VtxUV;

out vec2 UV;
void main()
{
    gl_Position = vec4(VtxPosition, 1.0f);
    UV = VtxUV;
}