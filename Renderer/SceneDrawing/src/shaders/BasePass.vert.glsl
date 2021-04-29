

#ifndef SKINNED_MESH
#define SKINNED_MESH 0
#endif

#ifndef NORMAL_MAP_TEXTURE
#define NORMAL_MAP_TEXTURE 0
#endif


// I could set it during linking or...
// REMEMBER IT HAS TO MATCH WITH VertexAttribute.hpp ENUM.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec4 aJoints;
layout (location = 5) in vec4 aJointWeights;

out VS_OUTPUT {
    vec3 Position;
#if NORMAL_MAP_TEXTURE
    mat3 NormalTangentSpace;
#else
    vec3 Normal;
#endif
    vec4 Silouette;
    vec2 TexCoords;
} OUT;

out gl_PerVertex
{
    vec4 gl_Position;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

#if SKINNED_MESH
uniform mat4 jointTransform[32];
#endif
uniform int mesh_id;

mat4 calculateModelTransform(in mat4 modelTransform);
#if NORMAL_MAP_TEXTURE
mat3 calculateNormalWorldTangentSpace(in mat4 modelTransform, in vec3 normal, in vec3 tangent);
#else
vec3 calculateWorldNormal(in mat4 modelTransform, in vec3 normal);
#endif
vec4 calculateSilhouette();
vec2 calculateTexCoords(in vec2 texCoords);

void main()
{
    mat4 modelTransform = calculateModelTransform(model);
    vec4 worldPos = modelTransform * vec4(aPos, 1);
    gl_Position = proj * view * worldPos;

    OUT.Position = worldPos.xyz;
#if NORMAL_MAP_TEXTURE
    OUT.NormalTangentSpace = calculateNormalWorldTangentSpace(modelTransform, aNormal, aTangent);
#else
    OUT.Normal = calculateWorldNormal(modelTransform, aNormal);
#endif
    OUT.Silouette = calculateSilhouette();
    OUT.TexCoords = calculateTexCoords(aTexCoords);
}

#if SKINNED_MESH
mat4 calculateModelTransform(in mat4 modelTransform) {
    mat4 skinMatrix = 
        aJointWeights.x * jointTransform[int(aJoints.x)] + 
        aJointWeights.y * jointTransform[int(aJoints.y)] + 
        aJointWeights.z * jointTransform[int(aJoints.z)] + 
        aJointWeights.w * jointTransform[int(aJoints.w)];

    return modelTransform * skinMatrix;
}
#else 
mat4 calculateModelTransform(in mat4 modelTransform) {
    return modelTransform;
}
#endif // SKINNED_MESH


#if NORMAL_MAP_TEXTURE
mat3 calculateNormalWorldTangentSpace(in mat4 modelTransform, in vec3 normal, in vec3 tangent) {
    
    vec3 T = normalize(tangent);
    vec3 N = normalize(normal);
    // Gram-Shmidt re-orthogonalize.
    T = normalize(T - (dot(T, N) * N));

    vec3 B = normalize(cross(T, N));

    return mat3(modelTransform) * transpose(mat3(T, B, N));
}
#else
vec3 calculateWorldNormal(in mat4 modelTransform, in vec3 fragNormal) {
    return normalize(modelTransform * vec4(fragNormal.xyz, 0)).xyz;
}
#endif

vec4 calculateSilhouette() {
    // Disassemble uint16 of mesh_id into values we put in contour map.
    float mesh_id_high = ((mesh_id >> 8) & 0xff)/255.f;
    float mesh_id_low = (mesh_id  & 0xff)/255.f;
    vec2 mesh_id_component = vec2(mesh_id_high, mesh_id_low);

    return vec4(0, 0, mesh_id_component);
}

vec2 calculateTexCoords(in vec2 texCoords) {
    // just forward...
    return texCoords;
}



