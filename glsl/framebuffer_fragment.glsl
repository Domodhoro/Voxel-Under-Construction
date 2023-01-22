#version 300 es

uniform sampler2D screenTexture;

out highp vec4 FragColor;
in  highp vec2 FragTexture;

uniform int Type;

void main() {
    highp vec3 color = texture(screenTexture, FragTexture).rgb;

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
