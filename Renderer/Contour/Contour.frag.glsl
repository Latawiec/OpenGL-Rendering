#version 330

in VS_OUTPUT {
    vec3 Position;
    vec3 Normal;
    vec4 ContourInfo;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} IN;

layout (location = 0) out vec3 PositionMap;
layout (location = 1) out vec4 ColorMap;
layout (location = 2) out vec3 NormalMap;
layout (location = 3) out vec4 ContourMap;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    PositionMap = IN.Position;
    ColorMap = (1.4f - ShadowCalculation(IN.FragPosLightSpace)) * vec4(1);
    NormalMap = IN.Normal;
    ContourMap = IN.ContourInfo ;
}
