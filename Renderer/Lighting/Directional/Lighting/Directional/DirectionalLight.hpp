#pragma once

#include <glm/glm.hpp>

#include <Common/DepthBuffer.hpp>

namespace Render {
namespace Lighting {
namespace Directional {

struct DirectionalLight {
    
    constexpr static int ShadowMapWidth = 1024;
    constexpr static int ShadowMapHeight = 1024;
    constexpr static float ShadowMapNearPlane = 1.f;
    constexpr static float ShadowMapFarPlane = 10.f;

    DirectionalLight();
    DirectionalLight(const glm::vec3 direction, const glm::vec4 color = { 1, 1, 1, 1 });

    void SetColor(const glm::vec4 color);
    const glm::vec4& GetColor() const;

    void SetDirection(const glm::vec3& direction);
    const glm::vec3& GetDirection() const;
    const glm::mat4& GetProjectionTransform() const;
    const glm::mat4& GetViewTransform() const;

    const Common::DepthBuffer& GetShadowMap() const;

private:
    glm::vec4 _color;
    glm::vec3 _direction;
    Common::DepthBuffer _shadowMap;
};
    
} // Render
} // Lighting   
} // Directional