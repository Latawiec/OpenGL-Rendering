
layout (location = 0) in vec3 aPos;

out vec2 TextureCoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{    
    gl_Position = vec4(aPos, 1);
    TextureCoord = (aPos.xy + vec2(1.0)) / vec2(2.0);
}
