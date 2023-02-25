#version 300 es

out highp vec4      FragColor;
in highp vec3       FragTexture;
uniform samplerCube Cube;

void main() {
    FragColor = texture(Cube, FragTexture);
}
