#version 330

in VS_OUTPUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 Edge;
    vec2 TexCoords;
} IN;

layout (location = 0) out vec3 Position;
layout (location = 1) out vec4 Color;
layout (location = 2) out vec3 Normal;
layout (location = 3) out vec4 Contour;

void main()
{
    Position = IN.FragPos;
    Contour = IN.Edge;
    Normal = IN.Normal;
    Color = vec4(Position, 1.0);
}
