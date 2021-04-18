#version 330
in vec2 FragPos;

out vec4 FragColor;

uniform sampler2D imageTexture;

void main() {
    vec2 normalizedPos = (FragPos + vec2(1.f))/2.f;
    FragColor = vec4(texture(imageTexture, normalizedPos).xyz, 1.f);
}