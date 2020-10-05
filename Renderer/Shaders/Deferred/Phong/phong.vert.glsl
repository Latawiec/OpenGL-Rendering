#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUTPUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} OUT;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 vColor[3] = vec3[](
    vec3(1,0,0),
    vec3(0,1,0),
    vec3(0,0,1)
);

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    OUT.FragPos = vColor[gl_VertexID % 3];//vec3(model * vec4(aPos, 1.0));
    OUT.TexCoords = aTexCoords;
    OUT.Normal = mat3(transpose(inverse(model))) * aNormal;
}
