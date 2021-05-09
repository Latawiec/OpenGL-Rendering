
in vec2 TextureCoord;

out vec3 FragColor;

uniform sampler2D silhouetteTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D depthTexture;

bool isSilhouetteEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV);
bool isDepthEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV);
bool isNormalMapEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV);

void main() {
    vec2 textureDim = textureSize(silhouetteTexture, 0);
    vec2 pixelPos = vec2(TextureCoord.x * textureDim.x, TextureCoord.y * textureDim.y);

    vec2 leftPixelPos = pixelPos - vec2(-1, 0);
    vec2 rightPixelPos = pixelPos - vec2(1, 0);
    vec2 topPixelPos = pixelPos - vec2(0, 1);
    vec2 botPixelPos = pixelPos - vec2(0, -1);

    vec2 leftUVPos = vec2(leftPixelPos.x / textureDim.x, leftPixelPos.y / textureDim.y);
    vec2 rightUVPos = vec2(rightPixelPos.x / textureDim.x, rightPixelPos.y / textureDim.y);
    vec2 topUVPos = vec2(topPixelPos.x / textureDim.x, topPixelPos.y / textureDim.y);
    vec2 botUVPos = vec2(botPixelPos.x / textureDim.x, botPixelPos.y / textureDim.y);



    FragColor = 
        //isDepthEdge(TextureCoord, leftUVPos, rightUVPos, topUVPos, botUVPos) ||
        isNormalMapEdge(TextureCoord, leftUVPos, rightUVPos, topUVPos, botUVPos) ||
        isSilhouetteEdge(TextureCoord, leftUVPos, rightUVPos, topUVPos, botUVPos)
     ? vec3(1, 1, 1) : vec3(0, 0, 0);
}


bool isSilhouetteEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV) {
    vec4 centerSample = texture(silhouetteTexture, centerUV);
    vec4 leftSample = texture(silhouetteTexture, leftUV);
    vec4 rightSample = texture(silhouetteTexture, rightUV);
    vec4 topSample = texture(silhouetteTexture, topUV);
    vec4 botSample = texture(silhouetteTexture, botUV);

    vec2 verticalDiff = (centerSample.zw - topSample.zw) + (centerSample.zw - botSample.zw);
    vec2 horizontalDiff = (centerSample.zw - leftSample.zw) + (centerSample.zw - rightSample.zw);

    return (verticalDiff.x + verticalDiff.y + horizontalDiff.x + horizontalDiff.y) > 0.0;
}

bool isDepthEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV)
{
    const float depthEdgeTreshold = 0.0001;

    // I'll invert it so that closer to view it is, higher the value.
    float centerSample = 1.0 - texture(depthTexture, centerUV).r;
    float leftSample = 1.0 - texture(depthTexture, leftUV).r;
    float rightSample = 1.0 - texture(depthTexture, rightUV).r;
    float topSample = 1.0 - texture(depthTexture, topUV).r;
    float botSample = 1.0 - texture(depthTexture, botUV).r;

    return (4 * centerSample - leftSample - rightSample - topSample - botSample) > depthEdgeTreshold;
}

bool isNormalMapEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV)
{
    const float normalEdgeTreshold = 0.85;
    vec3 centerSample = normalize(texture(normalMapTexture, centerUV).xyz);
    vec3 leftSample = normalize(texture(normalMapTexture, leftUV).xyz);
    vec3 rightSample = normalize(texture(normalMapTexture, rightUV).xyz);
    vec3 topSample = normalize(texture(normalMapTexture, topUV).xyz);
    vec3 botSample = normalize(texture(normalMapTexture, botUV).xyz);

    bool leftEdge = abs(dot(centerSample, leftSample)) < normalEdgeTreshold;
    bool rightEdge = abs(dot(centerSample, rightSample)) < normalEdgeTreshold;
    bool topEdge = abs(dot(centerSample, topSample)) < normalEdgeTreshold;
    bool botEdge = abs(dot(centerSample, botSample)) < normalEdgeTreshold;

    return leftEdge ||
        rightEdge ||
        topEdge ||
        botEdge;
}
