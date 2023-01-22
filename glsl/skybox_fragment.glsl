#version 300 es

out highp vec4 FragColor;

uniform samplerCube cube;

in highp vec3 FragTexture;

void main() {
    FragColor = texture(cube, FragTexture);
}
