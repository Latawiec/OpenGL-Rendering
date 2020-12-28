#include "Lighting/Directional/DirectionalLight.hpp"

namespace Render::Lighting::Directional {

DirectionalLight::DirectionalLight()
: _color({ 1, 1, 1, 1 })
, _direction({0, -1, 0})
, _shadowMap(ShadowMapWidth, ShadowMapHeight)
{}

DirectionalLight::DirectionalLight(const glm::vec3 direction, const glm::vec4 color)
: _color(color)
, _direction(direction)
, _shadowMap(ShadowMapWidth, ShadowMapHeight)
{}

void DirectionalLight::SetColor(const glm::vec4 color) {
    _color = color;
}

const glm::vec4& DirectionalLight::GetColor() const {
    return _color;
}

void DirectionalLight::SetDirection(const glm::vec3& direction) {
    _direction = direction;
}

const glm::vec3& DirectionalLight::GetDirection() const {
    return _direction;
}

const glm::mat4& DirectionalLight::GetProjectionTransform() const {
    return glm::ortho(-1.f, 1.f, -1.f, 1.f, ShadowMapNearPlane, ShadowMapFarPlane);
}

const glm::mat4& DirectionalLight::GetViewTransform() const {
    return glm::lookAt(_direction,
                       { 0.f, 0.f, 0.f },
                       { 0.f, 1.f, 0.f });
}

const Common::DepthBuffer& DirectionalLight::GetShadowMap() const {
    return _shadowMap;
}

} // Render::Lighting::Directional