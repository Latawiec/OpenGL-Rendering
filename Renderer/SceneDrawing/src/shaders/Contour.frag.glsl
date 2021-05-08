
in vec2 TextureCoord;

out vec3 FragColor;

uniform sampler2D silhouetteTexture;

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

    vec4 centerSample = texture(silhouetteTexture, TextureCoord);
    vec4 leftSample = texture(silhouetteTexture, leftUVPos);
    vec4 rightSample = texture(silhouetteTexture, rightUVPos);
    vec4 topSample = texture(silhouetteTexture, topUVPos);
    vec4 botSample = texture(silhouetteTexture, botUVPos);

    vec2 verticalDiff = (centerSample.zw - topSample.zw) + (centerSample.zw - botSample.zw);
    vec2 horizontalDiff = (centerSample.zw - leftSample.zw) + (centerSample.zw - rightSample.zw);

    bool isEdge = verticalDiff.x + verticalDiff.y + horizontalDiff.x + horizontalDiff.y > 0.0;

    FragColor = isEdge ? vec3(1, 1, 1) : vec3(0, 0, 0);
}
