#version 450 core
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require
#extension GL_NV_shader_buffer_load : require

layout (triangles) in;
layout (triangle_strip, max_vertices=3*6*4) out;

uniform mat4 ShadowMatrices[6*4];

out vec4 FragPos;
out uint LightIndex;

void main()
{
    
    int faceindex = 0;
    for(int light = 0;
        light < 4;
        light++) {
        for(int face = 0;
            face < 6;
            face++) {
            
            gl_Layer = faceindex;
            for(int i = 0; i < 3; i++){
                FragPos = gl_in[i].gl_Position;
                LightIndex = light;
                gl_Position = ShadowMatrices[faceindex] * FragPos;
                EmitVertex();
            }
            EndPrimitive();
            faceindex++;
        }
    }
}