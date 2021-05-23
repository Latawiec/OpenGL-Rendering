
#ifndef VERTICAL
#define VERTICAL 0
#endif

#ifndef HORIZONTAL
#define HORIZONTAL 0
#endif

in vec2 TextureCoord;
out vec3 FragColor;

uniform sampler2D sourceTexture;

vec3 GaussianBlur();

void main() {
    // I think I'll add different blurs one day. That's why it looks like this...
    FragColor = GaussianBlur();
}

vec3 GaussianBlur() {
    const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    vec2 texelSize = 1.0 / textureSize(sourceTexture, 0);
    vec3 result = texture(sourceTexture, TextureCoord).rgb * weight[0];

    #if HORIZONTAL
    vec2 offset = vec2(1.0, 0.0) * texelSize;
    #elif VERTICAL
    vec2 offset = vec2(0.0, 1.0) * texelSize;
    #else
    vec2 offset = vec2(0);
    #endif

    for (int i=1; i<5; ++i) {
        result += texture(sourceTexture, TextureCoord + offset * i).rgb * weight[i];
        result += texture(sourceTexture, TextureCoord - offset * i).rgb * weight[i];
    }

    return result;
}