#pragma once
#include <glm/glm.hpp>

namespace Render {
namespace Camera {

class BasicCamera {
    glm::vec3 _cameraPos = glm::vec3(-5.f, 0.f, -1.f);
    glm::vec3 _cameraFront = glm::vec3(1.f, 0.f, 0.f);
    const glm::vec3 _cameraUp = glm::vec3(0.f, 1.f, 0.f);

    float _yaw = 0.f;
    float _pitch = 0.f;

    float _fov = 45.f;
    float _aspectRatio = 1.f/2.f;

public:
    void updatePosition(const float front, const float left, const float up);

    void updateRotation(const float pitchOffset, const float yawOffset);

    void updateFov(const float fovOffset);

    void updateAspectRatio(const float newAspectRatio);

    glm::mat4 getViewTransform() const;

    glm::mat4 getProjectionTransform() const;

    glm::vec3 getPosition() const;

    glm::vec3 getFront() const;
};

} // namespace Camera
} // namespace Render