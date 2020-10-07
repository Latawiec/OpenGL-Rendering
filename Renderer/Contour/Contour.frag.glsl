#version 330

in VS_OUTPUT {
    vec3 Position;
    vec3 Normal;
    vec4 ContourInfo;
    vec2 TexCoords;
} IN;

layout (location = 0) out vec3 PositionMap;
layout (location = 1) out vec4 ColorMap;
layout (location = 2) out vec3 NormalMap;
layout (location = 3) out vec4 ContourMap;

void main()
{
    PositionMap = IN.Position;
    ColorMap = vec4(PositionMap, 1.0);
    NormalMap = IN.Normal;
    ContourMap = IN.ContourInfo;
}
