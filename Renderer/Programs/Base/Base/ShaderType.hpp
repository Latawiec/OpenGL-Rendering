#pragma once

#include <glad/glad.h>

namespace Renderer {
namespace Programs {
namespace Base {

enum class ShaderType : GLuint {
	Vertex = GL_VERTEX_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

} // namespace Base
} // namespace Programs
} // namespace Renderer