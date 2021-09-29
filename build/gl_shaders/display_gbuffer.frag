
layout(binding = 0)uniform sampler2D InResolve;
layout(binding = 1)uniform sampler2D InG0;
layout(binding = 2)uniform sampler2D InG1;
layout(binding = 3)uniform sampler2D InG2;
layout(binding = 4)uniform sampler2D InG3;
layout(binding = 5)uniform sampler2D InG4;
layout(binding = 6)uniform sampler2D InGDepth;
layout(binding = 7)uniform sampler2DArray InCascadeDepths;
layout(binding = 8)uniform samplerCubeArray InCubeShadowmap;

layout(location=0) out vec4 OutColor;

uniform uint PointLightIndex;
uniform int BufferToDisplay;
uniform vec2 Resolution;


//Returns current subrect of UV and scales UV to fit that Subrect
// 6  7  8
// 3  4  5
// 0  1  2
int DivideScreenReturnIndex(int ROWS, int COLUMNS, inout vec2 UV) {
    
    float X = float(COLUMNS);
    float Y = float(ROWS);
    float InvX = 1.0/X;
    float InvY = 1.0/Y;
    
    vec2 FinalUV = vec2(0);
    
    int TextureIndex = 0;
    int FinalTextureIndex = 0;
    
    for(int RowIndex = 0; RowIndex < ROWS; RowIndex++) {
        for(int ColumnIndex = 0; ColumnIndex < COLUMNS; ColumnIndex++) {       
            
            float TestX = InvX*float(ColumnIndex);
            float TestY = InvY*float(RowIndex);
            if(UV.x > TestX && UV.y > TestY) {
                FinalUV = UV;
                FinalUV -= vec2(TestX, TestY);
                FinalUV *= vec2(X, Y);
                FinalTextureIndex = TextureIndex;
            }
            TextureIndex++;
        }
    }
    UV = FinalUV;
    return FinalTextureIndex;
}

vec3 LinearizeAndDisplayDepth(vec2 UV) {
    
    float f = 50.0f;
    float n = 0.1;
    float z = (2 * n) / (f + n - texture(InGDepth, UV).r * (f - n));
    
    return vec3(z);
}

vec3 DisplayPointLightDepth(vec2 UV) {
    
    
    int MapIndex = DivideScreenReturnIndex(2, 3, UV);
    
    vec3 RayDir = vec3(0);
    
    if(MapIndex == 0) { RayDir = vec3(-1, -UV.y, UV.x);}
    if(MapIndex == 1) { RayDir = vec3(UV.x, -1, -UV.y);}
    if(MapIndex == 2) { RayDir = vec3(-UV.x, -UV.y, -1);}
    if(MapIndex == 3) { RayDir = vec3(1, -UV.y, -UV.x);}
    if(MapIndex == 4) { RayDir = vec3(UV.x, 1, UV.y);}
    if(MapIndex == 5) { RayDir = vec3(UV.x, -UV.y, 1);}
    
    float CubeShadowSample = texture(InCubeShadowmap, vec4(RayDir, PointLightIndex)).r;
    return vec3(CubeShadowSample);
}

vec3 DisplayCascadeDepth(vec2 UV) {
    
    float Index = 0.0;
    if(UV.x < 0.5) {
        if(UV.y < 0.5) {
            Index = 0.0;
            UV *= 2.0;
        } else {
            Index = 1.0;
            UV -= vec2(0, 0.5);
            UV *= 2.0;
        }
    } else {
        if(UV.y < 0.5) {
            Index = 2.0;
            UV -= vec2(0.5, 0);
            UV *= 2.0;
        } else {
            Index = 3.0;
            UV -= vec2(0.5, 0.5);
            UV *= 2.0;
        }
    }
    float CascadeSample = texture(InCascadeDepths, vec3(UV, Index)).r;
    
    return vec3(CascadeSample);
}

void main() {
    
    vec3 Color = vec3(1.0);
    vec2 UV = gl_FragCoord.xy / Resolution.xy;
    
    if(BufferToDisplay < 0) {
        
        float X = 3.0;
        float Y = 3.0;
        float InvX = 1.0/X;
        float InvY = 1.0/Y;
        
        if(UV.x < InvX) 
        {
            if(UV.y < InvY)
            {
                UV *= vec2(X, Y);
                Color = LinearizeAndDisplayDepth(UV);
            } 
            else if(UV.y < InvY*2.0) 
            {
                UV -= vec2(0, InvY);
                UV *= vec2(X, Y);
                Color = texture(InG2, UV).rgb;
            }
            else if(UV.y < InvY*3.0)
            {
                UV -= vec2(0, InvY*2.0);
                UV *= vec2(X, Y);
                Color = texture(InResolve, UV).rgb;
            }
        }
        else if(UV.x < InvX*2.0) 
        {
            if(UV.y < InvY)
            {
                UV -= vec2(InvX, 0);
                UV *= vec2(X, Y);
                Color = DisplayCascadeDepth(UV);
            } 
            else if(UV.y < InvY*2.0) 
            {
                UV -= vec2(InvX, InvY);
                UV *= vec2(X, Y);
                Color = texture(InG3, UV).rgb;
            }
            else if(UV.y < InvY*3.0)
            {
                UV -= vec2(InvX, InvY*2.0);
                UV *= vec2(X, Y);
                Color = texture(InG0, UV).rgb;
            }
        } else if(UV.x < InvX*3.0) {
            if(UV.y < InvY)
            {
                UV -= vec2(InvX*2.0, 0);
                UV *= vec2(X, Y);
                Color = DisplayPointLightDepth(UV);
            } 
            else if(UV.y < InvY*2.0) 
            {
                UV -= vec2(InvX*2.0, InvY);
                UV *= vec2(X, Y);
                Color = vec3(texture(InG4, UV).rg, 1.0);
            }
            else if(UV.y < InvY*3.0)
            {
                UV -= vec2(InvX*2.0, InvY*2.0);
                UV *= vec2(X, Y);
                Color = texture(InG1, UV).rgb;
            }
        }
    } else {
        
        if(BufferToDisplay == 0) { Color = texture(InResolve, UV).rgb;}
        if(BufferToDisplay == 1) { Color = texture(InG0, UV).rgb;}
        if(BufferToDisplay == 2) { Color = texture(InG1, UV).rgb;}
        if(BufferToDisplay == 3) { Color = texture(InG2, UV).rgb;}
        if(BufferToDisplay == 4) { Color = texture(InG3, UV).rgb;}
        if(BufferToDisplay == 5) { Color = vec3(texture(InG4, UV).rg, 1.0);}
        if(BufferToDisplay == 6) { Color = LinearizeAndDisplayDepth(UV);}
        if(BufferToDisplay == 7) { Color = DisplayCascadeDepth(UV);}
        if(BufferToDisplay == 8) { Color = DisplayPointLightDepth(UV);}
    }
    OutColor = vec4(Color, 1);
    
}