#pragma once 

#include <stdint.h>
#include <glm/glm.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

struct Camera { 
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;
    
    enum class Type : uint8_t {
        Perspective,
        Ortographic
    };

    Camera(const float fov, const float aspect, const glm::mat4& orientation);
    Camera(Camera&&) noexcept = default;
    Camera& operator=(Camera&&) noexcept = default;

    Camera(const Camera&) noexcept = default;
    Camera& operator=(const Camera&) noexcept = default;

    const glm::mat4& GetProjectionTransform() const;
    const glm::mat4& GetCameraOrientation() const;

private:
    glm::mat4 _cameraOrientation;
    glm::mat4 _projection;
    float _fov;
    float _aspectRatio;
    const Type _type;
};

} // namespace Base
} // namespace Scene 
} // namespace Renderer