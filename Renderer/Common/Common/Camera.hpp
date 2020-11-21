#pragma once 

#include <stdint.h>
#include <glm/glm.hpp>

namespace Render {
namespace Common {

struct Camera { 
    enum class Type : uint8_t {
        Perspective,
        Ortographic
    };

    Camera(const float fov, const float aspect, const glm::mat4& orientation);
    Camera(Camera&&) noexcept = default;
    Camera& operator=(Camera&&) noexcept = default;

    Camera(const Camera&) noexcept = default;
    Camera& operator=(const Camera&) noexcept = default;

    glm::mat4 GetProjectionTransform() const;
    glm::mat4 GetCameraOrientation() const;

private:
    glm::mat4 _cameraOrientation;
    float _fov;
    float _aspectRatio;
    const Type _type;
};

} // namespace Common 
} // namespace Render