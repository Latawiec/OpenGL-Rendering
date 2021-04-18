#include "../Scene/Base/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

Camera::Camera(const float fov, const float aspect, const glm::mat4& orientationMatrix = glm::mat4{1})
: _cameraOrientation{ orientationMatrix }
, _projection{ glm::perspective(fov, aspect, 0.1f, 100.f) }
, _fov(fov)
, _aspectRatio(aspect)
, _type(Type::Perspective) {}

const glm::mat4& Camera::GetProjectionTransform() const {
    return _projection;
}

const glm::mat4& Camera::GetCameraOrientation() const {
    return _cameraOrientation;
}

} // namespace Base
} // namespace Scene
} // namespace Renderer