#version 330
in vec2 FragPos;

out vec4 EdgesTexture;

const vec4 cChannelTresholds = vec4(0.4, 0.0, 0.0, 1.0);

uniform sampler2D edgeMap_tex;
uniform vec2 imageSize;

void main() {
    vec2 normalizedPos = (FragPos + vec2(1.f))/2.f;
    vec2 pixelPos = vec2(normalizedPos.x * imageSize.x, normalizedPos.y * imageSize.y);
    
    vec2 leftPixelPos = pixelPos - vec2(1, 0);
    vec2 topPixelPos = pixelPos - vec2(0, 1);

    vec2 leftNormalizedPos = vec2(leftPixelPos.x / imageSize.x, leftPixelPos.y / imageSize.y); 
    vec2 topNormalizedPos = vec2(topPixelPos.x / imageSize.x, topPixelPos.y / imageSize.y); 

    vec4 centerSample = texture(edgeMap_tex, normalizedPos);
    vec4 leftSample = texture(edgeMap_tex, leftNormalizedPos);
    vec4 topSample = texture(edgeMap_tex, topNormalizedPos);

    vec4 verticalDiff = abs(centerSample - topSample);
    vec4 horizontalDiff = abs(centerSample - leftSample);

    vec4 edgeTest = horizontalDiff + verticalDiff - cChannelTresholds;
    bool isEdge = max(edgeTest.x, max(edgeTest.y, max(edgeTest.z, edgeTest.w))) > 0.0;

    EdgesTexture = isEdge ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1);
}