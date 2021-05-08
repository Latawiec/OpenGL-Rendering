#include "../SceneDrawing/ShadowMappingPass/ShadowMappingPassBuffer.hpp"

namespace Renderer {
namespace SceneDrawing {
namespace ShadowMappingPass {

ShadowMappingPassBuffer::ShadowMappingPassBuffer(unsigned int shadowMapWidth, unsigned int shadowMapHeight)
: _shadowMapWidth(shadowMapWidth)
, _shadowMapHeight(shadowMapHeight)
{}

unsigned int ShadowMappingPassBuffer::GetDirectionalLightShadowTexture(const Scene::Base::DirectionalLight::IdType& id) const {
    return _directionalLightShadows.at(id).getTexture();
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

void ShadowMappingPassBuffer::SetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id, glm::mat4 transform) {
    _directionalLightTransforms[id] = transform;
}

const glm::mat4& ShadowMappingPassBuffer::GetDirectionalLightTransform(const Scene::Base::DirectionalLight::IdType& id) const {
    return _directionalLightTransforms.at(id);
}

} // namespace ShadowMappingPass
} // namespace SceneDrawing
} // namespace Renderer
