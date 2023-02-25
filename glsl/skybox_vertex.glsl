#version 300 es

layout(location = 0) in vec3 Position;

out highp vec3 FragTexture;
uniform mat4   View;
uniform mat4   Projection;

void main() {
    gl_Position = Projection * View * vec4(Position.xyz, 1.0f);
    FragTexture = Position;
}
