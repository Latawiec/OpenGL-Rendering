#include "../Scene/Base/SpotLight.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

SpotLight::SpotLight(const glm::mat4& orientation)
: _lightOrientation(orientation)
{}

void SpotLight::SetColor(const glm::vec3 color)
{
    _color = color;
}

const glm::vec3& SpotLight::GetColor() const
{
    return _color;
}

void SpotLight::SetIntensity(const float intensity)
{
    _intensity = intensity;
}

const float& SpotLight::GetIntensity() const
{
    return _intensity;
}

void SpotLight::SetCastingShadow(const bool castsShadow)
{
    _castsShadow = castsShadow;
}

bool SpotLight::IsCastingShadow() const
{
    return _castsShadow;
}

float SpotLight::GetInnerConeAngle() const
{
    return _innerConeAngle;
}

float SpotLight::GetOuterConeAngle() const
{
    return _outerConeAngle;
}

void SpotLight::SetInnerConeAngle(const float angle)
{
    _innerConeAngle = angle;
}

void SpotLight::SetOuterConeAngle(const float angle)
{
    _outerConeAngle = angle;
}

const glm::mat4& SpotLight::GetLightOrientation() const
{
    return _lightOrientation;
}

const glm::vec4& SpotLight::GetLightDirection() const
{
    return _lightDirection;
}

} // namespace Base
} // namespace Scene
} // namespace Renderer