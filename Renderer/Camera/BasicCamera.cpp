#include "BasicCamera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render {
namespace Camera {

void BasicCamera::updatePosition(const float front, const float left, const float up) {
    _cameraPos += _cameraFront * front;
    auto vecRight = glm::normalize(glm::cross(_cameraFront, _cameraUp));
    _cameraPos += vecRight * left;
    _cameraPos += _cameraUp * up;
}

void BasicCamera::updateRotation(const float pitchOffset, const float yawOffset) {
    _yaw += yawOffset;
    _pitch += pitchOffset;

    _pitch = _pitch > 89.f ? 89.f : _pitch;
    _pitch = _pitch < -89.f ? -89.f : _pitch;

    _cameraFront = glm::normalize(glm::vec3(
        cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
        sin(glm::radians(-_pitch)),
        sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
    ));
}

void BasicCamera::updateFov(const float fovOffset) {
    _fov -= fovOffset;
    if(_fov <= 1.0f)
        _fov = 1.0f;
    else if(_fov >= 45.0f)
        _fov = 45.0f;
}

void BasicCamera::updateAspectRatio(const float newAspectRatio) {
    _aspectRatio = newAspectRatio;   
}

glm::mat4 BasicCamera::getViewTransform() const {
    return glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
}

glm::mat4 BasicCamera::getProjectionTransform() const {
    return glm::perspective(glm::radians(_fov), _aspectRatio, 0.1f, 100.f);
}

glm::vec3 BasicCamera::getPosition() const {
    return _cameraPos;
}

glm::vec3 BasicCamera::getFront() const { 
    return _cameraFront;
}

} // namespace Camera
} // namespace Render