
#ifndef DIRECTIONAL_LIGHTS
#define DIRECTIONAL_LIGHTS 0
#endif

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D positionTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D metallicRoughnessTexture;

uniform vec4 cameraPosition;

#if DIRECTIONAL_LIGHTS
const uint MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE = 4;
uniform uint directionalLightCount = 0;
uniform mat4 directionalLightTransforms[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
uniform vec4 directionalLightDirections[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
uniform vec4 directionalLightColor[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
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

vec3 CalculateDiffuse_DirectionalLight(in vec2 coord, in uint lightIndex) {
    vec3 lightDirection = normalize(-directionalLightDirections[lightIndex].xyz);
    vec3 normal = normalize(texture(normalMapTexture, coord).xyz);

    return vec3(max(dot(lightDirection.xyz, normal), 0.0));
}

vec3 CalculateSpecular_DirectionalLight(in vec2 coord, in uint lightIndex) {
    vec3 lightDirection = normalize(-directionalLightDirections[lightIndex].xyz);
    vec4 fragmentPosition = vec4(texture(positionTexture, coord).xyz, 1.0);
    vec3 viewDirection = normalize((fragmentPosition - cameraPosition).xyz);
    vec3 normal = normalize(texture(normalMapTexture, coord).xyz);

    vec3 reflectDirection = reflect(lightDirection, normal);

    vec2 metallicRoughness = texture(metallicRoughnessTexture, coord).xy;

    // Mapping roughness/metallic from PBR to specular/glossiness... 
    float howShinyXD = mix(1.0, 9.0, 1.0 - metallicRoughness.y);
    float specularPower = sqrt(mix(0.0, 1.0, 1.0 - metallicRoughness.y));

    float specular = specularPower * pow(max(dot(viewDirection, reflectDirection), 0.0), pow(2, howShinyXD));
    return vec3(specular);
}

void main() {
    // for (int i=0; i<directionalLightCount; i++) {
        int i = 0;
        float shadow = CalculateShadow_DirectionalLight(TextureCoord, i);
        vec3 diffuse = CalculateDiffuse_DirectionalLight(TextureCoord, i);
        vec3 specular = CalculateSpecular_DirectionalLight(TextureCoord, i);

        vec3 lightColor = directionalLightColor[i].rgb;
        
        vec3 lighting = (1.0 - shadow) * diffuse * lightColor;
        vec3 spec =  (1.0 - shadow) * specular * lightColor;
    // }
    vec4 albedo = texture(albedoTexture, TextureCoord);
    FragColor = vec4(lighting, 1) * albedo + vec4(spec, 0);
}