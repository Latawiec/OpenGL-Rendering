#include "../Scene/Base/PointLight.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

PointLight::PointLight(const glm::mat4& orientation)
: _lightOrientation(orientation)
{}

void PointLight::SetColor(const glm::vec3 color)
{
    _color = color;
}

const glm::vec3& PointLight::GetColor() const
{
    return _color;
}

void PointLight::SetIntensity(const float intensity)
{
    _intensity = intensity;
}

const float& PointLight::GetIntensity() const
{
    return _intensity;
}

void PointLight::SetCastingShadow(const bool castsShadow)
{
    _castsShadow = castsShadow;
}

bool PointLight::IsCastingShadow() const
{
    return _castsShadow;
}

const glm::mat4& PointLight::GetLightOrientation() const
{
    return _lightOrientation;
}

} // namespace Base
} // namespace Scene
} // namespace Renderer