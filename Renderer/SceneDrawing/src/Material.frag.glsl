

#ifndef SHADOW_MAPPED
#define SHADOW_MAPPED 0
#endif

#ifndef BASE_COLOUR_TEXTURE
#define BASE_COLOUR_TEXTURE 0
#endif

#ifndef NORMAL_MAP_TEXTURE
#define NORMAL_MAP_TEXTURE 0
#endif

in VS_OUTPUT {
    vec3 Position;
    vec3 Normal;
    vec4 Silouette;
    vec2 TexCoords;
} IN;

layout (location = 0) out vec3 PositionMap;
layout (location = 1) out vec4 ColorMap;
layout (location = 2) out vec3 NormalMap;
layout (location = 3) out vec4 SilhouetteMap;

// uniform sampler2D shadowMap;
// uniform vec3 lightPosition;

#ifdef BASE_COLOUR_TEXTURE
uniform sampler2D baseColor;
#endif

#ifdef NORMAL_MAP_TEXTURE
uniform sampler2D normalMap;
#endif

vec4 SampleBaseColor(vec2 coords);
vec3 SampleNormalMap(vec2 coords);
vec4 SampleSilhouette(vec2 coords);

void main()
{
    PositionMap = IN.Position;
    ColorMap = SampleBaseColor(IN.TexCoords);
    NormalMap = SampleNormalMap(IN.TexCoords);
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


#ifdef NORMAL_MAP_TEXTURE
vec3 SampleNormalMap(vec2 coords)
{
    return IN.Normal;
}
#else
vec3 SampleNormalMap(vec2 coords)
{
    return IN.Normal;
}
#endif // NORMAL_MAP_TEXTURE

// float ShadowCalculation(vec4 fragPosLightSpace)
// {
//     // perform perspective divide
//     vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//     // transform to [0,1] range
//     projCoords = projCoords * 0.5 + 0.5;
//     // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//     float closestDepth = texture(shadowMap, projCoords.xy).r; 
//     // get depth of current fragment from light's perspective
//     float currentDepth = projCoords.z;
//     // calculate bias (based on depth map resolution and slope)
//     vec3 normal = normalize(IN.Normal);
//     vec3 lightDir = normalize(lightPosition - IN.Position);
//     float bias = max(0.015 * (1.0 - dot(normal, lightDir)), 0.005);
//     float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
//     if(projCoords.z > 1.0)
//         shadow = 0.0;
//     return shadow;
// }


