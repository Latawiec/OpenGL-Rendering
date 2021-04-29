#pragma once

#include <glad/glad.h>

namespace Renderer {
namespace Scene {
namespace Base {
    
/**
 * Vertex attributes with their their index asigned.
 */
enum VertexAttributeLocation : GLuint {
    POSITION = 0,
    NORMAL = 1,
    TANGENT = 2,
    UV_MAP = 3,
    JOINT = 4,
    JOINT_WEIGHT = 5,
    SIZE
};

} // namespace Base
} // namespace Scene
} // namespace Renderer