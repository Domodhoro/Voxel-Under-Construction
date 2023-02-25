#version 300 es

layout(location = 0) in vec3 Position;

uniform mat4 View;
uniform mat4 Projection;

out highp vec3 FragTexture;

void main() {
    gl_Position = Projection * View * vec4(Position.xyz, 1.0f);
    FragTexture = Position;
}
