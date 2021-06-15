layout (location = 0) in vec3 aPos;

out vec2 FragPos;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
    gl_Position = vec4(aPos, 1);
    FragPos = aPos.xy;
}