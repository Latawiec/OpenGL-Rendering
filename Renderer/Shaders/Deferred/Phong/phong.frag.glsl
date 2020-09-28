#version 330

in VS_OUTPUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} IN;

layout (location = 0) out vec3 Position;
layout (location = 1) out vec4 Color;
layout (location = 2) out vec3 Normal;

void main()
{
    Position = IN.FragPos;
    Color = vec4(IN.TexCoords, 0.0, 1.0);
    Normal = IN.Normal;
}
