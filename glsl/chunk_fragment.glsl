#version 300 es

uniform sampler2D TextureSampler;

out highp vec4 FragColor;
in  highp vec3 FragTexture;

highp vec2 tex_sample(highp vec2 tex, highp float size, highp float x, highp float y) {
    return vec2((tex.x + x) * size, (tex.y + y) * size);
}

void main() {
    highp vec4 color = vec4(1.0f);
    highp float size = 0.25f;

    if      (FragTexture.z == 1.0f) color = texture(TextureSampler, tex_sample(FragTexture.xy, size , 0.0f, 0.0f));
    else if (FragTexture.z == 2.0f) color = texture(TextureSampler, tex_sample(FragTexture.xy, size , 1.0f, 0.0f));
    else if (FragTexture.z == 3.0f) color = texture(TextureSampler, tex_sample(FragTexture.xy, size , 2.0f, 0.0f));
    else if (FragTexture.z == 4.0f) color = texture(TextureSampler, tex_sample(FragTexture.xy, size , 3.0f, 0.0f));
    else if (FragTexture.z == 5.0f) color = texture(TextureSampler, tex_sample(FragTexture.xy, size , 0.0f, 1.0f));
    else if (FragTexture.z == 6.0f) color = texture(TextureSampler, tex_sample(FragTexture.xy, size , 1.0f, 1.0f));
    else if (FragTexture.z == 7.0f) color = texture(TextureSampler, tex_sample(FragTexture.xy, size , 2.0f, 1.0f));

    if (color.r == 1.0f && color.g != 1.0f && color.b == 1.0f) discard;

    FragColor = color;
}
