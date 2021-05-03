
in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main() {
    
    vec3 albedo = texture(albedoTexture, TextureCoord).rgb;
    vec3 diffuse = texture(diffuseTexture, TextureCoord).rgb;
    vec3 specular = texture(specularTexture, TextureCoord).rgb;

    FragColor = vec4(diffuse * albedo + specular, 1);
}