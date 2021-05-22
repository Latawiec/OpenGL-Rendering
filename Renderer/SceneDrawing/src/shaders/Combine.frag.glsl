
in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D ditherTexture;
uniform sampler2D contourTexture;

vec3 GammaCorrection(in vec3 linearColor);
vec3 ToneMapping(in vec3 hdrColor, in float exposure);

void main() {
    
    vec3 albedo = texture(albedoTexture, TextureCoord).rgb;
    vec3 diffuse = texture(diffuseTexture, TextureCoord).rgb;
    vec3 specular = texture(specularTexture, TextureCoord).rgb;
    float dither = texture(ditherTexture, TextureCoord).r;
    //FragColor = vec4(diffuse * albedo + specular, 1);
    float diffuseAvg = (diffuse.r + diffuse.g + diffuse.b) / 3.0;
    float specularAvg = (specular.r + specular.g + specular.b) / 3.0;
    float diffuseMax = max(max(diffuse.r, diffuse.g), diffuse.b);
    vec3 contour = texture(contourTexture, TextureCoord).rgb;


    //vec3 output = vec3(max(vec3(0), vec3(ceil(diffuseAvg - dither) + ceil(specularAvg - dither))) + contour);

    // Some minimum HDR and exposure-based tonemapping.
    float exposure = 1.5;

    vec3 outColor = GammaCorrection(ToneMapping(vec3(diffuseMax + specularAvg), exposure));
    FragColor = vec4(ceil(outColor - dither) + contour, 1);

}

vec3 GammaCorrection(in vec3 hdrColor) 
{
    const float gamma = 2.2;
    return pow(hdrColor, vec3(1.0 / gamma));
}

vec3 ToneMapping(in vec3 hdrColor, in float exposure)
{
    return vec3(1.0) - exp(-hdrColor * exposure);
}