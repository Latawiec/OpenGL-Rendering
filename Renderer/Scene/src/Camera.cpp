#include "../Scene/Base/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <projection.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

Camera::Camera(const float near, const float far, const float fov, const float aspect, const glm::mat4& orientationMatrix = glm::mat4{1})
: _cameraOrientation{ orientationMatrix }
, _projection{ Utils::perspective(fov, aspect, near, far) }
, _near(near)
, _far(far)
, _fov(fov)
, _aspectRatio(aspect)
, _type(Type::Perspective) {}

const glm::mat4& Camera::GetProjectionTransform() const {
    return _projection;
}

const glm::mat4& Camera::GetCameraOrientation() const {
    return _cameraOrientation;
}

float Camera::GetNear() const {
    return _near;
}

float Camera::GetFar() const {
    return _far;
}

float Camera::GetFov() const {
    return _fov;
}

float Camera::GetAspectRatio() const {
    return _aspectRatio;
}


} // namespace Base
} // namespace Scene
} // namespace Renderer