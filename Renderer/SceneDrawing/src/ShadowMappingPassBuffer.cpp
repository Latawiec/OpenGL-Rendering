#include "../SceneDrawing/ShadowMappingPass/ShadowMappingPassBuffer.hpp"

namespace Renderer {
namespace SceneDrawing {
namespace ShadowMappingPass {

ShadowMappingPassBuffer::ShadowMappingPassBuffer(unsigned int shadowMapWidth, unsigned int shadowMapHeight)
: _shadowMapWidth(shadowMapWidth)
, _shadowMapHeight(shadowMapHeight)
{}

void ShadowMappingPassBuffer::SetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id, glm::mat4 transform) {
    _directionalLightTransforms[id] = transform;
}

const glm::mat4& ShadowMappingPassBuffer::GetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id) const {
    return _directionalLightTransforms.at(id);
}

const DepthBuffer& ShadowMappingPassBuffer::GetDirectionalLightShadowRenderTarget(const Scene::Base::DirectionalLight::IdType& id) {
    if (!_directionalLightShadows.contains(id)) {
        _directionalLightShadows.emplace(id, DepthBuffer(_shadowMapWidth, _shadowMapHeight));
    }
    return _directionalLightShadows.at(id);
}

void ShadowMappingPassBuffer::RemoveDirectionalShadowRenderTarget(const Scene::Base::DirectionalLight::IdType& id) {
    _directionalLightShadows.erase(id);
}


void ShadowMappingPassBuffer::SetPointLightTransform(const Scene::Base::PointLight::IdType& id, glm::mat4 transform) {
    _pointLightTransforms[id] = transform;
}

const glm::mat4& ShadowMappingPassBuffer::GetPointLightTransform(const Scene::Base::PointLight::IdType& id) const {
    return _pointLightTransforms.at(id);
}

const CubeDepthBuffer& ShadowMappingPassBuffer::GetPointLightShadowRenderTarget(const Scene::Base::PointLight::IdType& id) {
    if (!_pointLightShadows.contains(id)) {
        _pointLightShadows.emplace(id, CubeDepthBuffer(_shadowMapWidth, _shadowMapHeight));
    }
    return _pointLightShadows.at(id);
}

void ShadowMappingPassBuffer::RemovePointLightShadowRenderTarget(const Scene::Base::PointLight::IdType& id) {
    _pointLightShadows.erase(id);
}


} // namespace ShadowMappingPass
} // namespace SceneDrawing
} // namespace Renderer
