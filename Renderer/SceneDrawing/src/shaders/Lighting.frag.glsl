
#ifndef DIRECTIONAL_LIGHTS
#define DIRECTIONAL_LIGHTS 0
#endif

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D positionTexture;
uniform sampler2D normalMapTexture;

#if DIRECTIONAL_LIGHTS
const uint MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE = 4;
uniform uint directionalLightCount = 0;
uniform mat4 directionalLightTransforms[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
uniform vec4 directionalLightDirections[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
uniform sampler2D directionalLightShadowmapSamplers[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
#endif

float CalculateShadow_DirectionalLight(in vec2 coord, in uint lightIndex) {
    mat4 lightTransform = directionalLightTransforms[lightIndex];

    vec4 fragPos_WorldSpace = vec4(texture(positionTexture, coord).xyz, 1.0);
    vec4 fragPos_LightSpace_Perspective = lightTransform * fragPos_WorldSpace;
    vec3 fragPos_LightSpace = fragPos_LightSpace_Perspective.xyz / fragPos_LightSpace_Perspective.w;
    if(fragPos_LightSpace.z > 1.0)
        return 1.0f;

    fragPos_LightSpace = fragPos_LightSpace * 0.5 + 0.5;
    
    float shadowmapDepth = texture(directionalLightShadowmapSamplers[lightIndex], fragPos_LightSpace.xy).r;
    float fragmentDepth = fragPos_LightSpace.z;

    vec3 normal = texture(normalMapTexture, coord).xyz;

    float shadowBias = 0.005;
    float shadow = fragmentDepth - shadowBias > shadowmapDepth ? 1.0f : 0.0f;

    return shadow;
}

float CalculateDiffuse_DirectionalLight(in vec2 coord, in uint lightIndex) {
    vec3 lightDirection = normalize(-directionalLightDirections[lightIndex].xyz);
    vec3 normal = normalize(texture(normalMapTexture, coord).xyz);

    return max(dot(lightDirection.xyz, normal), 0.0);
}

void main() {
    // for (int i=0; i<directionalLightCount; i++) {
        int i = 0;
        float shadow = CalculateShadow_DirectionalLight(TextureCoord, i);
        float diffuse = CalculateDiffuse_DirectionalLight(TextureCoord, i);

        vec3 color = vec3((1.0 - shadow) * diffuse);
        FragColor = vec4(color, 1);
    // }
}