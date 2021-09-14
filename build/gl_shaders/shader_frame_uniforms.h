
#define PI32 3.14159265359
#define TAU32 6.283185307179

struct directional_light {
    vec3 Direction;
    vec3 Color;
};

struct point_light {
    vec3 Position;
    vec3 Color;
    float Constant;
    float Linear;
    float Quadratic;
};

layout(std140, row_major, binding=0)uniform FrameUniforms 
{
    mat4 CameraProj;
    mat4 LightProj;
    point_light PointLight[4];
    directional_light SunLight;
    vec3 CameraP;
    uint FrambufferTextureToDisplay;
};