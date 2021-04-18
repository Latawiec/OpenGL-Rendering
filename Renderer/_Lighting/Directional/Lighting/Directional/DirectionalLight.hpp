#pragma once

#include <glm/glm.hpp>

#include <Common/DepthBuffer.hpp>

namespace Render {
namespace Lighting {
namespace Directional {

/**
 * Simple directional light (Sun light) description.
 * 
 * It is a light that has no defined source. Light affecting an object always has
 * the same vector, without care about objects position.
 * 
 * Light always shines from this->GetDirection().
 */
struct DirectionalLight {

    DirectionalLight();
    DirectionalLight(const glm::vec3 direction, const glm::vec4 color = { 1, 1, 1, 1 });

    void SetColor(const glm::vec4 color);
    const glm::vec4& GetColor() const;

    void SetDirection(const glm::vec3& direction);
    const glm::vec3& GetDirection() const;

    void SetCastingShadow(bool castsShadow);
    bool IsCastingShadow() const;

private:
    glm::vec4 _color;
    glm::vec3 _direction;
    bool _castsShadow = false;
};
    
} // Render
} // Lighting   
} // Directional