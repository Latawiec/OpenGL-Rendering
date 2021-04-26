
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
    vec4 lightDirection = directionalLightDirections[lightIndex];

    vec4 fragPos_WorldSpace = vec4(texture(positionTexture, coord).xyz, 1.0);
    vec4 fragPos_LightSpace_Perspective = lightTransform * fragPos_WorldSpace;
    vec3 fragPos_LightSpace = fragPos_LightSpace_Perspective.xyz / fragPos_LightSpace_Perspective.w;
    if(fragPos_LightSpace.z > 1.0)
        return 0.0f;
        
    fragPos_LightSpace = fragPos_LightSpace * 0.5 + 0.5;
    
    float shadowmapDepth = texture(directionalLightShadowmapSamplers[lightIndex], fragPos_LightSpace.xy).r;
    float fragmentDepth = fragPos_LightSpace.z;

    vec3 normal = texture(normalMapTexture, coord).xyz;

    float shadowBias = 0.005;//max(0.05 * (1.0 - dot(normal, lightDirection.xyz)), 0.005);
    float shadow = fragmentDepth - shadowBias > shadowmapDepth ? 0.0f : 1.0f;

    return shadow;
}

void main() {
    // for (int i=0; i<directionalLightCount; i++) {
        int i = 0;
        vec3 shadows = vec3(CalculateShadow_DirectionalLight(TextureCoord, i));
        //FragColor = vec4((directionalLightTransforms[0] * texture(positionTexture, TextureCoord)).xyz, 1);   
        FragColor = vec4(shadows, 1);
    // }
}