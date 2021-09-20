#version 450 core
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require
#extension GL_NV_shader_buffer_load : require

layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[3];
};

void main()
{          
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = 
            lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}  