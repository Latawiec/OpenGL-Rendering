
#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT 0
#endif

#ifndef SKINNED_MESH
#define SKINNED_MESH 0
#endif

layout (location = 0) in vec3 aPos;
#if SKINNED_MESH
layout (location = 4) in vec4 aJoints;
layout (location = 5) in vec4 aJointWeights;
#endif

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

vec4 calculateWorldPosition(in mat4 modelTransform, in vec3 fragPos);

void main()
{
    vec4 worldPosition = calculateWorldPosition(model, aPos);
    gl_Position = proj * view * worldPosition;
}

#if SKINNED_MESH
vec4 calculateWorldPosition(in mat4 modelTransform, in vec3 fragPos) {
    mat4 skinMatrix = 
        aJointWeights.x * jointTransform[int(aJoints.x)] + 
        aJointWeights.y * jointTransform[int(aJoints.y)] + 
        aJointWeights.z * jointTransform[int(aJoints.z)] + 
        aJointWeights.w * jointTransform[int(aJoints.w)];

    return modelTransform * skinMatrix * vec4(fragPos, 1.0);
}
#else 
vec4 calculateWorldPosition(in mat4 modelTransform, in vec3 fragPos) {
    // Just transform by model.
    return vec4(modelTransform * vec4(aPos, 1.0));
}
#endif // SKINNED_MESH

