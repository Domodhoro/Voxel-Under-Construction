#version 300 es

uniform sampler2D screenTexture;

out highp vec4 FragColor;
in  highp vec2 FragTexture;

void main() {
    highp vec3 color = texture(screenTexture, FragTexture).rgb;
    FragColor        = vec4(color, 1.0);

    //highp vec3 color = vec3(1.0 - texture(screenTexture, FragTexture)).rbg;
    //FragColor        = vec4(color, 1.0);

    //FragColor           = texture(screenTexture, FragTexture);
    //highp float average = 0.5f * (FragColor.r + FragColor.g + FragColor.b);
    //FragColor           = vec4(average, average, average, 1.0);
}
