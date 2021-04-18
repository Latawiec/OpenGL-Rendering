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
    UV_MAP = 2,
    JOINT = 3,
    JOINT_WEIGHT = 4,
    SIZE
};

} // namespace Base
} // namespace Scene
} // namespace Renderer