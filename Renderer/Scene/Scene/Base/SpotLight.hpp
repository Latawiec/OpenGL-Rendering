#pragma once 

#include <stdint.h>
#include <glm/glm.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

struct SpotLight {
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    SpotLight(const glm::mat4& orientation);

    void SetColor(const glm::vec3 color);
    const glm::vec3& GetColor() const;

    void SetIntensity(const float intensity);
    const float& GetIntensity() const;

    void SetCastingShadow(const bool castsShadow);
    bool IsCastingShadow() const;

    float GetInnerConeAngle() const;
    float GetOuterConeAngle() const;
    void SetInnerConeAngle(const float angle);
    void SetOuterConeAngle(const float angle);

    const glm::mat4& GetLightOrientation() const;
    const glm::vec4& GetLightDirection() const;

private:
    // It always points down. The only thing that can change it would be node above it and it's transforms.
    const glm::vec4 _lightDirection { 0, -1, 0, 0 };
    glm::mat4 _lightOrientation;
    glm::vec3 _color = glm::vec3(1);
    float _intensity = 1.f;
    float _innerConeAngle = 0.f;
    float _outerConeAngle = 0.f;
    bool _castsShadow = false;
};

} // namespace Base
} // namespace Scene 
} // namespace Renderer