
in vec4 FragPos;

uniform vec3 LightP;
uniform float FarPlane;

void main()
{
    float LightDistance = length(FragPos.xyz - LightP);
    
    //map to [0-1] by dividing by far plance
    LightDistance = LightDistance / FarPlane;
    
    //Built in frag depth
    gl_FragDepth = LightDistance;
}