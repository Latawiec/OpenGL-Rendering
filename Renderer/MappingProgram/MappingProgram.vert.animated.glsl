#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aEdgeColours;
layout (location = 4) in vec4 aJoints;
layout (location = 5) in vec4 aJointWeights;

out VS_OUTPUT {
    vec3 Position;
    vec3 Normal;
    vec4 ContourMap;
    vec2 TexCoords;
} OUT;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 jointTransform[32];

uniform int mesh_id;

vec4 calculateWorldSkinnedPosition(in mat4 modelTransform, in vec3 fragPos);
vec4 calculateWorldPosition(in mat4 modelTransform, in vec3 fragPos);
vec3 calculateNormal(in mat4 modelTransform, in vec3 fragNormal);
vec4 calculateContourMap();
vec2 calculateTexCoords(in vec2 texCoords);

void main()
{
    vec4 worldPosition = calculateWorldSkinnedPosition(model, aPos);
    gl_Position = proj * view * worldPosition;
    OUT.Position = worldPosition.xyz;
    OUT.Normal = calculateNormal(model, aNormal);
    OUT.ContourMap = calculateContourMap();
    OUT.TexCoords = calculateTexCoords(aTexCoords);
}

vec4 calculateWorldSkinnedPosition(in mat4 modelTransform, in vec3 fragPos) {
    mat4 skinMatrix = 
        aJointWeights.x * jointTransform[int(aJoints.x)] + 
        aJointWeights.y * jointTransform[int(aJoints.y)] + 
        aJointWeights.z * jointTransform[int(aJoints.z)] + 
        aJointWeights.w * jointTransform[int(aJoints.w)];

    return model * vec4(fragPos, 1.0);
}

vec4 calculateWorldPosition(in mat4 modelTransform, in vec3 fragPos) {
    return vec4(modelTransform * vec4(aPos, 1.0));
}

vec3 calculateNormal(in mat4 modelTransform, in vec3 fragNormal) {
    return normalize(mat3(transpose(inverse(modelTransform))) * fragNormal);
}

// Soon this will be fixed. I'll apply texture instead of per-vertex colour.
vec4 calculateContourMap() {
    // Disassemble uint16 of mesh_id into values we put in contour map.
    float mesh_id_high = ((mesh_id >> 8) & 0xff)/255.0;
    float mesh_id_low = (mesh_id  & 0xff)/255.0;
    vec2 mesh_id_component = vec2(mesh_id_high, mesh_id_low);

    return vec4(0, 0, mesh_id_component);
}

vec2 calculateTexCoords(in vec2 texCoords) {
    // just forward...
    return texCoords;
}



