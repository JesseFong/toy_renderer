
layout(location=0) out vec4 OutColor;
in vec3 TexCoords;

uniform samplerCube Skybox;

void main(){
    OutColor = texture(Skybox, TexCoords);
    //OutColor = vec4(1, 0, 1, 1);
}