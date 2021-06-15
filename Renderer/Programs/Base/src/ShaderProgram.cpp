#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <stdint.h>
#include <string_view>
#include <vector>

#include "../Base/ShaderCompiler.hpp"


namespace Renderer {
namespace Programs {
namespace Base {

template<ShaderType Type>
ShaderProgram<Type>::ShaderProgram(const GLuint compiledShaderProgram)
: _id(compiledShaderProgram) {}
	
template<ShaderType Type>
ShaderProgram<Type>::ShaderProgram(ShaderProgram&& other) {
    std::swap(_id, other._id);
}

template<ShaderType Type>
ShaderProgram<Type>& ShaderProgram<Type>::operator=(ShaderProgram&& other) {
    std::swap(_id, other._id);
    return *this;
}

template<ShaderType Type>
ShaderProgram<Type>::operator GLuint() const {
    return _id;
}

template<ShaderType Type>
GLuint ShaderProgram<Type>::GetId() const {
    return _id;
}

template<ShaderType Type>
ShaderProgram<Type>::~ShaderProgram() {
    if (_id != -1) {
        glDeleteProgram(_id);
    }
}

// Instance
template ShaderProgram<ShaderType::Vertex>;
template ShaderProgram<ShaderType::Fragment>;
template ShaderProgram<ShaderType::Geometry>;

} // namespace Base
} // namespace Programs
} // namespace Renderer