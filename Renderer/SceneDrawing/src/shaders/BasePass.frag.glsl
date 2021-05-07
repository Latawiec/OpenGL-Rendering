

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

#ifndef DITHERING
#define DITHERING 0
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
layout (location = 5) out float Dither;

#ifdef BASE_COLOUR_TEXTURE
uniform sampler2D baseColor;
#endif

#if NORMAL_MAP_TEXTURE
uniform sampler2D normalMap;
#endif

#if METALLIC_ROUGHNESS_TEXTURE
uniform sampler2D metallicRoughness;
#endif

#if DITHERING
uniform sampler2D ditherTexture;
#endif

vec4 SampleBaseColor(in vec2 coords);
vec3 SampleNormalMap(in vec2 coords);
vec4 SampleSilhouette(in vec2 coords);
vec2 SampleRoughnessMetallic(in vec2 coords);
float SampleDither(in vec2 fragPos);

void main()
{
    PositionMap = IN.Position;
    ColorMap = SampleBaseColor(IN.TexCoords);
    NormalMap = SampleNormalMap(IN.TexCoords);
    RoughnessMetallic = SampleRoughnessMetallic(IN.TexCoords);
    SilhouetteMap = IN.Silouette;
    Dither = SampleDither(gl_FragCoord.xy);
}


vec4 SampleBaseColor(vec2 coords) 
{
#ifdef BASE_COLOUR_TEXTURE
    return texture(baseColor, coords.xy);
#else
    return vec4(1);
#endif
}


vec3 SampleNormalMap(vec2 coords)
{
#if NORMAL_MAP_TEXTURE
    vec3 normal = texture(normalMap, coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    // TODO: Just revert it when importing texture. No need to eat cycles here each frame.
    normal *= vec3(1, -1, 1);
    return normalize(IN.NormalTangentSpace * normal);
#else
    return IN.Normal;
#endif
}


vec2 SampleRoughnessMetallic(in vec2 coords) 
{
#if METALLIC_ROUGHNESS_TEXTURE
    vec2 metallicRoughness = texture(metallicRoughness, coords).bg;
    return metallicRoughness;
#else
    return vec2(0, 1);
#endif
}


float SampleDither(in vec2 fragPos)
{
#if DITHERING
    vec2 dim = textureSize(ditherTexture, 0);
    // We assume that Dither texture is repeating.
    return texture(ditherTexture, fragPos/dim).r;
#else
    return 0.5;
#endif
}
