
#ifndef DIRECTIONAL_LIGHTS
#define DIRECTIONAL_LIGHTS 0
#endif

#ifndef SPOT_LIGHTS
#define SPOT_LIGHTS 0
#endif

in vec2 TextureCoord;

layout (location = 0) out vec3 DiffuseTexture;
layout (location = 1) out vec3 SpecularTexture;

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
uniform vec4 directionalLightColors[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
uniform sampler2D directionalLightShadowmapSamplers[MAX_DIRECTIONAL_LIGHTS_PER_EXECUTE];
#endif

#if SPOT_LIGHTS
const uint MAX_SPOT_LIGHTS_PER_EXECUTE = 4;
uniform uint spotLightCount = 0;
uniform mat4 spotLightTransforms[MAX_SPOT_LIGHTS_PER_EXECUTE];
uniform vec4 spotLightDirections[MAX_SPOT_LIGHTS_PER_EXECUTE];
uniform vec4 spotLightPositions[MAX_SPOT_LIGHTS_PER_EXECUTE];
uniform vec4 spotLightColors[MAX_SPOT_LIGHTS_PER_EXECUTE];
uniform vec2 spotLightInnerOuterConeAngles[MAX_SPOT_LIGHTS_PER_EXECUTE];
uniform sampler2D spotLightShadowmapSamplers[MAX_SPOT_LIGHTS_PER_EXECUTE];
#endif

#if DIRECTIONAL_LIGHTS
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
    float howShinyXD = mix(0.0, 9.0, 1.0 - metallicRoughness.y);
    float specularPower = mix(0.0, 1.0, 1.0 - metallicRoughness.y);

    float specular = specularPower * pow(max(dot(viewDirection, reflectDirection), 0.0), pow(2, howShinyXD));
    return vec3(specular);
}
#endif

#if SPOT_LIGHTS
float CalculateShadow_SpotLight(in vec2 coord, in uint lightIndex) {
    mat4 lightTransform = spotLightTransforms[lightIndex];

    vec4 fragPos_WorldSpace = vec4(texture(positionTexture, coord).xyz, 1.0);
    vec4 fragPos_LightSpace_Perspective = lightTransform * fragPos_WorldSpace;
    vec3 fragPos_LightSpace = fragPos_LightSpace_Perspective.xyz / fragPos_LightSpace_Perspective.w;

    if (fragPos_LightSpace.z > 1.0)
        return 1.0f;

    fragPos_LightSpace = fragPos_LightSpace * 0.5 + 0.5;

    float shadowmapDepth = texture(spotLightShadowmapSamplers[lightIndex], fragPos_LightSpace.xy).r;
    float fragmentDepth = fragPos_LightSpace.z;

    float shadowBias = 0.005;
    float shadow = (fragmentDepth - shadowBias > shadowmapDepth) ? 1.0f : 0.0f;

    return shadow;
}

vec3 CalculateDiffuse_SpotLight(in vec2 coord, in uint lightIndex) {
    vec3 lightPos = spotLightPositions[lightIndex].xyz;
    vec3 fragPos = texture(positionTexture, coord).xyz;
    vec3 lightToFragDir = normalize(fragPos - lightPos);
    vec3 lightDir = normalize(spotLightDirections[lightIndex].xyz);

    vec2 lightInnerOuterConeAngles = spotLightInnerOuterConeAngles[lightIndex];

    float cosAngle = dot(lightToFragDir, lightDir);
    float innerConeCosAngle = cos(lightInnerOuterConeAngles.x);
    float outerConeCosAngle = cos(lightInnerOuterConeAngles.y);

    float falloffMultiplier = clamp(1.0 - ( (cosAngle - innerConeCosAngle)/(outerConeCosAngle - innerConeCosAngle) ), 0, 1);

    vec3 normal = normalize(texture(normalMapTexture, coord).xyz);
    return falloffMultiplier * vec3(max(dot(-lightDir, normal), 0.0));
}

vec3 CalculateSpecular_SpotLight(in vec2 coord, in uint lightIndex) {
    return vec3(0);
}
#endif

void main() {
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);
    
#if DIRECTIONAL_LIGHTS
    for (int i=0; i<directionalLightCount; i++) {

        float shadowFactor = CalculateShadow_DirectionalLight(TextureCoord, i);
        vec3 diffuseFactor = CalculateDiffuse_DirectionalLight(TextureCoord, i);
        vec3 specularFactor = CalculateSpecular_DirectionalLight(TextureCoord, i);
        // I keep intensity in Alpha channel.
        vec3 lightColor = directionalLightColors[i].rgb * directionalLightColors[i].a;
        
        diffuse += (1.0 - shadowFactor) * diffuseFactor * lightColor;
        specular += (1.0 - shadowFactor) * specularFactor * lightColor;
    }
#endif

#if SPOT_LIGHTS
    for (int i=0; i<spotLightCount; i++) {
        
        float shadowFactor = CalculateShadow_SpotLight(TextureCoord, i);
        vec3 diffuseFactor = CalculateDiffuse_SpotLight(TextureCoord, i);
        vec3 specularFactor = CalculateSpecular_SpotLight(TextureCoord, i);
        // I keep intensity in Alpha channel.
        vec3 lightColor = spotLightColors[i].rgb * spotLightColors[i].a;

        diffuse += (1.0 - shadowFactor) * diffuseFactor * lightColor;
        specular += (1.0 - shadowFactor) * specularFactor * lightColor;
    }
#endif

    DiffuseTexture = diffuse;
    SpecularTexture = specular;
}