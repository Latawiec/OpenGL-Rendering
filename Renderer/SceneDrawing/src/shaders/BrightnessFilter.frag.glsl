
#ifndef NULLIFY_BELOW
#define NULLIFY_BELOW 0
#endif

in vec2 TextureCoord;
out vec3 FragColor;

uniform sampler2D sourceTexture;
uniform float brightnessTreshold = 1.0f;

vec3 Filter();

void main() {
    FragColor = Filter();
}

vec3 Filter() {
    const vec3 brightnessWeights = vec3(0.2126, 0.7152, 0.0722);

    vec3 color = texture(sourceTexture, TextureCoord).rgb;
    float brightness = dot(color, brightnessWeights);
    vec3 result = vec3(0);

    #if NULLIFY_BELOW
    result = brightness > brightnessTreshold ? color : vec3(0);
    #endif

    return result;
}