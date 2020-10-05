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
    bool isEdge = min(min(IN.FragPos.x, IN.FragPos.y), IN.FragPos.z) < 0.01;
    if (isEdge) {
        Color = vec4(1, 1, 1, 1.0);
    }
    Normal = IN.Normal;
}
