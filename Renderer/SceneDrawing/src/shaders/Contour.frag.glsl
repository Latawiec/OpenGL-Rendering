
in vec2 TextureCoord;

out vec3 FragColor;

uniform sampler2D silhouetteTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D depthTexture;

uniform vec2 cameraNearFar;

bool isSilhouetteEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV);
bool isDepthEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV);
bool isNormalMapEdge(in vec2 centerUV, in vec2 leftUV, in vec2 rightUV, in vec2 topUV, in vec2 botUV);

void main() {
    vec2 textureDim = textureSize(silhouetteTexture, 0);
    vec2 pixelPos = vec2(TextureCoord.x * textureDim.x, TextureCoord.y * textureDim.y);

    //Directions:                             left         right       top         bot
    const vec2 sampleDirections[] = vec2[4](vec2(-1, 0), vec2(1, 0), vec2(0, 1), vec2(0, -1));

    vec4 centerSilhouette = texture(silhouetteTexture, TextureCoord);
    vec3 centerNormal = normalize(texture(normalMapTexture, TextureCoord).rgb);
    float centerDepth = texture(depthTexture, TextureCoord).r;

    // It has to reach at least 1.0 or we don't draw the edge for this fragment.
    // Even if some test doesn't result in an "is edge" result, it only contributes to the overall contourWeight.
    float contourWeight = 0.0;

    for (int i=0; i<4; ++i) {
        vec2 dir = sampleDirections[i];
        vec2 pixelPos = pixelPos - dir;
        vec2 uvPos = pixelPos / textureDim;

        float sampleDepth = texture(depthTexture, uvPos).r;

        // Test 0
        // I only draw edge outside from objects closer to the camera. So I draw on elements further away from camera.
        // I wanted to multiply by depth-fuse, but actually just abort mission if depth test failed. Less texture sampling.
        float depthFuse = centerDepth < sampleDepth ? 1.0 : 0.0;
        if (depthFuse == 0.0) continue;

        // Test 1 - silhouette test. If silhouette texture is any different, it is an edge 100%.
        vec4 sampleSilhouette = texture(silhouetteTexture, uvPos);
        vec4 silhouetteDiff = centerSilhouette - sampleSilhouette;
        float silhouetteWeight = silhouetteDiff.x + silhouetteDiff.y + silhouetteDiff.z + silhouetteDiff.w != 0 ? 1.0 : 0.0;

        // Test 2 - depth test. Anything below treshold is not edge, anything above is an edge. We'll linearize depth for it.
        const float depthEdgeTreshold = 0.005;
        const float depthExponent = 0.1; // bump lower values so when we're close, we see edge sooner.
        float near = cameraNearFar.x;
        float far = cameraNearFar.y;
        float centerDepthLin =  2.0 * near / (far + near - (2.0 * centerDepth - 1.0) * (far - near));
        float sampleDepthLin =  2.0 * near / (far + near - (2.0 * sampleDepth - 1.0) * (far - near));

        float depthDiff = abs(pow(sampleDepthLin, depthExponent) - pow(centerDepthLin, depthExponent));
        float depthWeight = depthDiff / (depthEdgeTreshold);

        // Test 3 - normal test. Treshold again, using dot product.
        const float normalEdgeTreshold = 0.4;
        vec3 sampleNormal = normalize(texture(normalMapTexture, uvPos).rgb);
        float dotProduct = dot(centerNormal, sampleNormal);
        // For x == 1, f(x) == 0;
        // For x == t, f(x) == 1;
        // ax + b and we get this...
        float normalWeight = max((1.0 / (normalEdgeTreshold - 1.0)) * (dotProduct - 1.0), 0.0);

        contourWeight += depthFuse * (
            silhouetteWeight +
            depthWeight +
            normalWeight +
            0.0
        );
    }

    FragColor = contourWeight >= 1.0 ? vec3(1, 1, 1) : vec3(0, 0, 0);
}
