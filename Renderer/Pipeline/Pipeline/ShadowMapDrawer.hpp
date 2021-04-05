#pragma once 

#include <map>

#include "DrawingExecutor.hpp"
#include <Common/DepthBuffer.hpp>
#include <Common/Mesh.hpp>

namespace Render {

class ShadowMapDrawer {

public:

    ShadowMapDrawer(const Common::Scene& scene);
    
    void Draw() {

        // Directional Light
        const auto& shadowMaps = scene.GetDirectionalShadowMaps();
        for (const auto& [nodeId, lightId] : scene.GetDirectionalLightNodes()) {
            const bool castsShadow = shadowMaps.contains(lightId);
            if (castsShadow) {
                const auto& shadowMap = scene.GetDirectionalShadow(lightId);
                const auto& node = scene.GetNode(nodeId);
                const auto& light = scene.GetDirectionalLight(lightId);
                drawDirectionalShadowMap(node, light, shadowMap);
            }
        }



        for (const auto& [shadowMap, view, projection] : _lights) {
            Common::FramebufferBase::ScopedBinding bind(shadowMap);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_DEPTH_BUFFER_BIT);
            glCullFace(GL_FRONT);
            Draw(view, projection);
        }
    }

private:
    void drawDirectionalShadowMap(const Node& node, const Directional::Light&, const DepthBuffer& shadowMap) {
        
    }
};

} // namespace Render