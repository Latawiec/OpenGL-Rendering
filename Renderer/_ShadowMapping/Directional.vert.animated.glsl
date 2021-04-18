#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec4 aJoints;
layout (location = 4) in vec4 aJointWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

vec4 calculateWorldSkinnedPosition(in mat4 modelTransform, in vec3 fragPos);

void main()
{
    gl_Position = proj * view * calculateWorldSkinnedPosition(model, aPos);
}

vec4 calculateWorldSkinnedPosition(in mat4 modelTransform, in vec3 fragPos) {
    mat4 skinMatrix = 
        aJointWeights.x * jointTransform[int(aJoints.x)] + 
        aJointWeights.y * jointTransform[int(aJoints.y)] + 
        aJointWeights.z * jointTransform[int(aJoints.z)] + 
        aJointWeights.w * jointTransform[int(aJoints.w)];

    return model * skinMatrix * vec4(fragPos, 1.0);
}



