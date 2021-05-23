

#ifndef ADDITIVE
#define ADDITIVE 0
#endif

in vec2 TextureCoord;
out vec3 FragColor;

const uint MAX_BLEND_SOURCES_PER_EXECUTE = 4;
uniform sampler2D sourceSamplers[MAX_BLEND_SOURCES_PER_EXECUTE];
uniform uint sourcesCount = 0;

vec3 AdditiveBlend();

void main() {
    #if ADDITIVE
    FragColor = AdditiveBlend();
    #else
    FragColor = vec3(1, 0, 1);
    #endif
}

vec3 AdditiveBlend() {
    vec3 result = texture(sourceSamplers[0], TextureCoord).rgb;
    for (int i=1; i<sourcesCount; ++i) {
        result += texture(sourceSamplers[i], TextureCoord).rgb;
    }
    return result;
}