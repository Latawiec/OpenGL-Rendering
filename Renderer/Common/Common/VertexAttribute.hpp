#pragma once

#include <glad/glad.h>

namespace Render {
namespace Common {
    
/**
 * Vertex attributes with their their index asigned.
 */
enum VertexAttribute : GLuint {
    POSITION = 0,
    NORMAL = 1,
    UV_MAP = 2,
    JOINT = 3,
    JOINT_WEIGHT = 4,
    SIZE
};

} // namespac Common
} // namespac Render