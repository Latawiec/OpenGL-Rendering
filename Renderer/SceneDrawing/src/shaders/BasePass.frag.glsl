

#ifndef SHADOW_MAPPED
#define SHADOW_MAPPED 0
#endif

#ifndef BASE_COLOUR_TEXTURE
#define BASE_COLOUR_TEXTURE 0
#endif

#ifndef NORMAL_MAP_TEXTURE
#define NORMAL_MAP_TEXTURE 0
#endif

#ifndef METALLIC_ROUGHNESS_TEXTURE
#define METALLIC_ROUGHNESS_TEXTURE 0
#endif

in VS_OUTPUT {
    vec3 Position;
#if NORMAL_MAP_TEXTURE
    mat3 NormalTangentSpace;
#else
    vec3 Normal;
#endif
    vec4 Silouette;
    vec2 TexCoords;
} IN;

layout (location = 0) out vec3 PositionMap;
layout (location = 1) out vec4 ColorMap;
layout (location = 2) out vec3 NormalMap;
layout (location = 3) out vec2 RoughnessMetallic;
layout (location = 4) out vec4 SilhouetteMap;

#ifdef BASE_COLOUR_TEXTURE
uniform sampler2D baseColor;
#endif

#if NORMAL_MAP_TEXTURE
uniform sampler2D normalMap;
#endif

#if METALLIC_ROUGHNESS_TEXTURE
uniform sampler2D metallicRoughness;
#endif

vec4 SampleBaseColor(in vec2 coords);
vec3 SampleNormalMap(in vec2 coords);
vec4 SampleSilhouette(in vec2 coords);
vec2 SampleRoughnessMetallic(in vec2 coords);

void main()
{
    PositionMap = IN.Position;
    ColorMap = SampleBaseColor(IN.TexCoords);
    NormalMap = SampleNormalMap(IN.TexCoords);
    RoughnessMetallic = SampleRoughnessMetallic(IN.TexCoords);
    SilhouetteMap = IN.Silouette;

    //ColorMap = (1.4f - ShadowCalculation(IN.FragPosLightSpace)) * vec4(1);
}


#ifdef BASE_COLOUR_TEXTURE
vec4 SampleBaseColor(vec2 coords) 
{
    return texture(baseColor, coords.xy);
}
#else
vec4 SampleBaseColor(vec2 coords) 
{
    return vec4(1);
}
#endif // BASE_COLOUR_TEXTURE


#if NORMAL_MAP_TEXTURE
vec3 SampleNormalMap(vec2 coords)
{
    vec3 normal = texture(normalMap, coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    // TODO: Just revert it when importing texture. No need to eat cycles here each frame.
    normal *= vec3(1, -1, 1);
    return normalize(IN.NormalTangentSpace * normal);
}
#else
vec3 SampleNormalMap(vec2 coords)
{
    return IN.Normal;
}
#endif // NORMAL_MAP_TEXTURE

#if METALLIC_ROUGHNESS_TEXTURE
vec2 SampleRoughnessMetallic(in vec2 coords) 
{
    vec2 metallicRoughness = texture(metallicRoughness, coords).bg;
    return metallicRoughness;
}
#else
vec2 SampleRoughnessMetallic(in vec2 coords) 
{
    return vec2(0);
}
#endif

