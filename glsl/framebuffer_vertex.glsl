#version 300 es

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 Texture;

out highp vec2 FragTexture;

void main() {
    gl_Position = vec4(Position.xy, 0.0f, 1.0f);
    FragTexture = Texture;
}
