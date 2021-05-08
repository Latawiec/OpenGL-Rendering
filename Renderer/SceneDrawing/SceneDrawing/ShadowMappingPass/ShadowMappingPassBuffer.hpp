#pragma once

#include <Scene/Base/DirectionalLight.hpp>
#include "../DepthBuffer.hpp"

#include <cstddef>
#include <utility>
#include <unordered_map>

namespace Renderer {
namespace SceneDrawing {
namespace ShadowMappingPass {

class ShadowMappingPassBuffer {

public:
    ShadowMappingPassBuffer(unsigned int shadowMapWidth, unsigned int shadowMapHeight);

    unsigned int GetDirectionalLightShadowTexture(const Scene::Base::DirectionalLight::IdType& id) const;
    const DepthBuffer& GetDirectionalLightShadowRenderTarget(const Scene::Base::DirectionalLight::IdType& id);
    void RemoveDirectionalShadowRenderTarget(const Scene::Base::DirectionalLight::IdType& id);

    void SetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id, glm::mat4 transform);
    const glm::mat4& GetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id) const;

private:
    unsigned int _shadowMapWidth;
    unsigned int _shadowMapHeight;

    std::unordered_map<Scene::Base::DirectionalLight::IdType, DepthBuffer> _directionalLightShadows;
    std::unordered_map<Scene::Base::DirectionalLight::IdType, glm::mat4> _directionalLightTransforms;

    void setupDirectionalShadowRenderTarget();
};

} // namespace ShadowMappingPass
} // namespace SceneDrawing
} // namespace Renderer