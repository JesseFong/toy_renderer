#version 450 core
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require
#extension GL_NV_shader_buffer_load : require

layout(triangles, invocations = 4) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 LightSpaceMatrices[16];

void main()
{          
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = 
            LightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}  