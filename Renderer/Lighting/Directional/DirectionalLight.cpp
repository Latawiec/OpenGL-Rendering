#include "Lighting/Directional/DirectionalLight.hpp"

namespace Render::Lighting::Directional {

DirectionalLight::DirectionalLight()
: _color({ 1, 1, 1, 1 })
, _direction({0, -1, 0})
{}

DirectionalLight::DirectionalLight(const glm::vec3 direction, const glm::vec4 color)
: _color(color)
, _direction(direction)
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

} // Render::Lighting::Directional