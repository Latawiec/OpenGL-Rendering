#include "Common/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Render {
namespace Common {

Camera::Camera(const float fov, const float aspect, const glm::mat4& orientationMatrix = glm::mat4{1})
: _cameraOrientation(orientationMatrix)
, _projection(glm::perspective(_fov, _aspectRatio, 0.1f, 100.f))
, _fov(fov)
, _aspectRatio(aspect)
, _type(Type::Perspective) {}

const glm::mat4& Camera::GetProjectionTransform() const {
    return _projection;
}

const glm::mat4& Camera::GetCameraOrientation() const {
    return _cameraOrientation;
}

} // namespace Common
} // namespace Render