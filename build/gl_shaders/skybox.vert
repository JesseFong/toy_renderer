
layout (location = 0) in vec3 VtxPosition;

out vec3 TexCoords;

uniform mat4 Proj;

void main(){
    TexCoords = VtxPosition;
    vec4 FinalPosition = Proj * vec4(VtxPosition, 1.0);
    FinalPosition.z = FinalPosition.w - 0.0001;
    gl_Position = FinalPosition;
}