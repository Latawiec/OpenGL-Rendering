#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>

namespace Render {
namespace Common {

template<class T>
struct Transformed {
    glm::mat4 transform;
    const T mesh;
};

// I need those a lot so...
using TransformedMesh = Transformed<Mesh&>;

} // namespace Common
} // namespace Render