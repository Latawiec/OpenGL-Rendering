#include "Common/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Render {
namespace Common {

Camera::Camera(const float fov, const float aspect)
: _fov(fov)
, _aspectRatio(aspect)
, _type(Type::Perspective) {}

glm::mat4 Camera::GetProjectionTransform() const {
    // Don't care about ortographic for now.
    // Also, I should be able to only generate this matrix if FoV, Aspect Ratio or type changes,
    // instead of every single time this matrix is queried...
    return glm::perspective(_fov, _aspectRatio, 0.1f, 100.f);
}

} // namespace Common
} // namespace Render