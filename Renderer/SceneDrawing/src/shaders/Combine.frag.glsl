
in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D ditherTexture;

void main() {
    
    vec3 albedo = texture(albedoTexture, TextureCoord).rgb;
    vec3 diffuse = texture(diffuseTexture, TextureCoord).rgb;
    vec3 specular = texture(specularTexture, TextureCoord).rgb;
    float dither = texture(ditherTexture, TextureCoord).r;
    //FragColor = vec4(diffuse * albedo + specular, 1);
    float diffuseAvg = (diffuse.r + diffuse.g + diffuse.b) / 3.0;
    float specularAvg = (specular.r + specular.g + specular.b) / 3.0;
    float diffuseMax = max(max(diffuse.r, diffuse.g), diffuse.b);
    FragColor = vec4(vec3(round(diffuseAvg - dither) + round(specularAvg - dither)), 1);
}