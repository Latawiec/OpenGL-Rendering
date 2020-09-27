#include "ShaderProgram.hpp"

namespace Render {

template<>
void ShaderProgram::set<bool>(const std::string_view name, const bool value) {
	glUniform1i(glGetUniformLocation(_id, name.data()), (int)value);
}

template<>
void ShaderProgram::set<int>(const std::string_view name, const int value) {
	glUniform1i(glGetUniformLocation(_id, name.data()), value);
}

template<>
void ShaderProgram::set<unsigned int>(const std::string_view name, const unsigned int value) {
	glUniform1i(glGetUniformLocation(_id, name.data()), value);
}

template<>
void ShaderProgram::set<float>(const std::string_view name, const float value) {
	glUniform1f(glGetUniformLocation(_id, name.data()), value);
}

template<>
void ShaderProgram::set<glm::mat4>(const std::string_view name, const glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(_id, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void ShaderProgram::set<glm::vec3>(const std::string_view name, const glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(_id, name.data()), 1, glm::value_ptr(value));
}

} // namespace Render