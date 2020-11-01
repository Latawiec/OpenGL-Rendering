#pragma once 

#include <map>

#include "DrawingExecutor.hpp"
#include <Common/DepthBuffer.hpp>
#include <Shadow/Program.hpp>
#include <Common/Mesh.hpp>

namespace Render {

class ShadowMapDrawer : public DrawingExecutor<Shadow::Program, Common::Mesh> {

    struct ShadowMapInfo {
        ShadowMapInfo(const Common::DepthBuffer& depthBuffer, const glm::mat4& view, const glm::mat4& proj)
        : shadowMap(depthBuffer), viewTransform(view), projectionTransform(proj) {}
        const Common::DepthBuffer& shadowMap;
        const glm::mat4 viewTransform;
        const glm::mat4 projectionTransform;
    };

    std::vector<ShadowMapInfo> _lights;    

public:
    void AddLight(const Common::DepthBuffer& shadowMap, const glm::mat4& view, const glm::mat4& projection) {
        _lights.emplace_back(shadowMap, view, projection);
    }

    void DrawShadowMaps() {
        for (const auto& [shadowMap, view, projection] : _lights) {
            Common::FramebufferBase::ScopedBinding bind(shadowMap);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_DEPTH_BUFFER_BIT);
            glCullFace(GL_FRONT);
            Draw(view, projection);
        }
    }

    void ClearLights() {
        _lights.clear();
    }

    const std::vector<ShadowMapInfo>& GetShadowMapsInfo() const {
        return _lights;
    }
};

} // namespace Render