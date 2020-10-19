#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aEdgeColours;

out VS_OUTPUT {
    vec3 Position;
    vec3 Normal;
    vec4 ContourInfo;
    vec2 TexCoords;
} OUT;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int mesh_id;

void main()
{
    // Disassemble uint16 of mesh_id into values we put in contour map.
    float mesh_id_high = ((mesh_id >> 8) & 0xff)/255.0;
    float mesh_id_low = (mesh_id  & 0xff)/255.0;
    vec2 mesh_id_component = vec2(mesh_id_high, mesh_id_low);

    gl_Position = proj * view * model * vec4(aPos, 1.0);

    OUT.Position = vec3(model * vec4(aPos, 1.0));
    OUT.Normal = mat3(transpose(inverse(model))) * aNormal;
    //OUT.ContourInfo = aEdgeColours;
    OUT.ContourInfo = vec4(0, mesh_id_component, 1.0);
    OUT.TexCoords = aTexCoords;
}
