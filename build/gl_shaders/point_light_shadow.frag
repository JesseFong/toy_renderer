
in vec4 FragPos;
flat in uint LightIndex;
uniform vec3 LightP[4];
uniform float FarPlane[4];

void main()
{
    float LightDistance = length(FragPos.xyz - LightP[LightIndex]);
    
    //map to [0-1] by dividing by far plance
    LightDistance = LightDistance / FarPlane[LightIndex];
    
    //Built in frag depth
    gl_FragDepth = LightDistance;
}