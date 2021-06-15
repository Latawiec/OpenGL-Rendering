#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <stdint.h>
#include <string_view>
#include <vector>

#include "ShaderType.hpp"


namespace Renderer {
namespace Programs {
namespace Base {

template<ShaderType Type>
class ShaderProgram {
	GLint _id = -1;
public:

	ShaderProgram() = default;
	ShaderProgram(const GLuint compiledShaderProgram);
	~ShaderProgram();

	ShaderProgram(ShaderProgram&& other);
	ShaderProgram& operator=(ShaderProgram&& other);

	ShaderProgram(ShaderProgram& other) = delete;
	ShaderProgram& operator=(ShaderProgram& other) = delete;

	operator GLuint() const;
	GLuint GetId() const;

	struct ScopedBinding {
        ScopedBinding(const ShaderProgram& shaderProgram) { glUseProgram(shaderProgram._id); }
        ~ScopedBinding() { glUseProgram(0); }
    };

};

} // namespace Base
} // namespace Programs
} // namespace Renderer