#version 300 es

uniform sampler2D TextureSampler;

out highp vec4 FragColor;

in highp vec3 FragTexture;

highp vec2 Sample(highp vec2 Texture, highp float Size, highp float x, highp float y);

void main() {
    highp vec4 Color = vec4(1.0f);

    if (FragTexture.z == 1.0f) {
        Color = texture(TextureSampler, Sample(FragTexture.xy, 0.25f , 0.0f, 0.0f));
    } else if (FragTexture.z == 2.0f) {
        Color = texture(TextureSampler, Sample(FragTexture.xy, 0.25f , 1.0f, 0.0f));
    } else if (FragTexture.z == 3.0f) {
        Color = texture(TextureSampler, Sample(FragTexture.xy, 0.25f , 2.0f, 0.0f));
    }

    if (Color.r == 1.0f && Color.g != 1.0f && Color.b == 1.0f) {
        discard;
    }

    FragColor = Color;
}

highp vec2 Sample(highp vec2 Texture, highp float Size, highp float x, highp float y) {
    return vec2((Texture.x + x) * Size, (Texture.y + y) * Size);
}
