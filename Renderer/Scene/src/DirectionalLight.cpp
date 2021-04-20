#include "../Scene/Base/DirectionalLight.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

DirectionalLight::DirectionalLight(const glm::mat4& orientation)
: _lightOrientation(orientation)
{}

void DirectionalLight::SetColor(const glm::vec3 color)
{
    _color = color;
}

const glm::vec3& DirectionalLight::GetColor() const
{
    return _color;
}

void DirectionalLight::SetIntensity(const float intensity)
{
    _intensity = intensity;
}

const float& DirectionalLight::GetIntensity() const
{
    return _intensity;
}

void DirectionalLight::SetCastingShadow(const bool castsShadow)
{
    _castsShadow = castsShadow;
}

bool DirectionalLight::IsCastingShadow() const
{
    return _castsShadow;
}

const glm::mat4& DirectionalLight::GetLightOrientation() const
{
    return _lightOrientation;
}

const glm::vec4& DirectionalLight::GetLightDirection() const
{
    return _lightDirection;
}

} // namespace Base
} // namespace Scene
} // namespace Renderer