#version 300 es

uniform sampler2D screenTexture;

out highp vec4 FragColor;
in  highp vec2 FragTexture;

uniform int Type;

const highp float off_set = 0.01f;

void main() {
    highp vec3 color = vec3(0.0f);

    const highp vec2 off_sets[9] = vec2[9] (
        vec2(-off_set,  off_set),
        vec2(    0.0f,  off_set),
        vec2( off_set,  off_set),
        vec2(-off_set,     0.0f),
        vec2(    0.0f,     0.0f),
        vec2( off_set,     0.0f),
        vec2(-off_set, -off_set),
        vec2(    0.0f, -off_set),
        vec2( off_set, -off_set)
    );

    const highp float kernel[9] = float[9] (
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    );

    highp vec3 sample_tex[9];

    for (int i = 0; i != 9; ++i) sample_tex[i]  = vec3(texture(screenTexture, FragTexture.st + off_sets[i]));
    for (int i = 0; i != 9; ++i) color         += sample_tex[i] * kernel[i];

    switch (Type) {
    case 0:
        FragColor = vec4(color, 1.0f);
        break;
    case 1:
        color     = vec3(1.0f - color);
        FragColor = vec4(color, 1.0f);
        break;
    case 2:
        FragColor = vec4(color, 1.0f);
        FragColor = vec4(vec3(0.5f * (color.r + color.g + color.b)), 1.0f);
        break;
    }
}
