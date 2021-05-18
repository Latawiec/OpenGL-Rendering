#pragma once 

#include <stdint.h>
#include <glm/glm.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

struct PointLight {
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    PointLight(const glm::mat4& orientation);

    void SetColor(const glm::vec3 color);
    const glm::vec3& GetColor() const;

    void SetIntensity(const float intensity);
    const float& GetIntensity() const;

    void SetCastingShadow(const bool castsShadow);
    bool IsCastingShadow() const;

    const glm::mat4& GetLightOrientation() const;

private:
    glm::mat4 _lightOrientation;
    glm::vec3 _color = glm::vec3(1);
    float _intensity = 1.f;
    bool _castsShadow = false;
};

} // namespace Base
} // namespace Scene 
} // namespace Renderer