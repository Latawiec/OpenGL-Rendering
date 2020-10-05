#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUTPUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 Edge;
    vec2 TexCoords;
} OUT;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int mesh_id;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    OUT.FragPos = vec3(model * vec4(aPos, 1.0));
    OUT.TexCoords = aTexCoords;
    OUT.Edge = vec4(0, ((mesh_id >> 8) & 0xff)/255.0, (mesh_id  & 0xff)/255.0, 1.0);
    OUT.Normal = mat3(transpose(inverse(model))) * aNormal;
}
