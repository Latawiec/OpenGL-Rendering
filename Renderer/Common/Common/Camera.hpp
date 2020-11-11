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

    Camera(const float fov, const float aspect);
    Camera(Camera&&) noexcept = default;
    Camera& operator=(Camera&&) noexcept = default;

    Camera(const Camera&) noexcept = default;
    Camera& operator=(const Camera&) noexcept = default;

    glm::mat4 GetProjectionTransform() const;

private:
    float _fov;
    float _aspectRatio;
    const Type _type;
};

} // namespace Common 
} // namespace Render