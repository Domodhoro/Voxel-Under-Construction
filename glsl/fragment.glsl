#version 300 es

uniform sampler2D TextureSampler;

out highp vec4 FragColor;

in highp vec2 FragTexture;

void main() {
    highp vec4 Color = texture(TextureSampler, FragTexture);

    if (Color.r == 1.0f && Color.g != 1.0f && Color.b == 1.0f) {
        discard;
    }

    FragColor = Color;
}
