
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1);
}