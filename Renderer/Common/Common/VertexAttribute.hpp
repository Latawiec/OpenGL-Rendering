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
    EDGE_INFO = 3,
    JOINT = 4,
    JOINT_WEIGHT = 5,
    SIZE
};

} // namespac Common
} // namespac Render