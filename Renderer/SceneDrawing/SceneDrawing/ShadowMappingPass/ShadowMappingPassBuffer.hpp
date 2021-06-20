#pragma once

#include <Scene/Base/DirectionalLight.hpp>
#include <Scene/Base/PointLight.hpp>
#include <Scene/Base/SpotLight.hpp>
#include "../DepthBuffer.hpp"
#include "../CubeDepthBuffer.hpp"

#include <cstddef>
#include <utility>
#include <unordered_map>

namespace Renderer {
namespace SceneDrawing {
namespace ShadowMappingPass {

class ShadowMappingPassBuffer {

public:
    ShadowMappingPassBuffer(unsigned int shadowMapWidth, unsigned int shadowMapHeight);

    // Directional
    void SetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id, glm::mat4 transform);
    const glm::mat4& GetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id) const;

    const DepthBuffer& GetDirectionalLightShadowRenderTarget(const Scene::Base::DirectionalLight::IdType& id);
    void RemoveDirectionalShadowRenderTarget(const Scene::Base::DirectionalLight::IdType& id);

    // Point
    void SetPointLightTransform(const Scene::Base::PointLight::IdType& id, glm::mat4 transform);
    const glm::mat4& GetPointLightTransform(const Scene::Base::PointLight::IdType& id) const;

    const CubeDepthBuffer& GetPointLightShadowRenderTarget(const Scene::Base::PointLight::IdType& id);
    void RemovePointLightShadowRenderTarget(const Scene::Base::PointLight::IdType& id);

    // Spot
    void SetSpotLightTransform(const Scene::Base::SpotLight::IdType& id, glm::mat4 transform);
    const glm::mat4& GetSpotLightTransform(const Scene::Base::PointLight::IdType& id) const;

    const DepthBuffer& GetSpotLightShadowRenderTarget(const Scene::Base::SpotLight::IdType& id);
    void RemoveSpotLightShadowRenderTarget(const Scene::Base::SpotLight::IdType& id);

private:
    unsigned int _shadowMapWidth;
    unsigned int _shadowMapHeight;

    std::unordered_map<Scene::Base::DirectionalLight::IdType, DepthBuffer> _directionalLightShadows;
    std::unordered_map<Scene::Base::DirectionalLight::IdType, glm::mat4> _directionalLightTransforms;

    std::unordered_map<Scene::Base::PointLight::IdType, CubeDepthBuffer> _pointLightShadows;
    std::unordered_map<Scene::Base::PointLight::IdType, glm::mat4> _pointLightTransforms;

    std::unordered_map<Scene::Base::SpotLight::IdType, DepthBuffer> _spotLightShadows;
    std::unordered_map<Scene::Base::SpotLight::IdType, glm::mat4> _spotLightTransforms;

    void setupDirectionalShadowRenderTarget();
};

} // namespace ShadowMappingPass
} // namespace SceneDrawing
} // namespace Renderer