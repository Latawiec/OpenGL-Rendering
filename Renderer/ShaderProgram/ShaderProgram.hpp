#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <stdint.h>
#include <string_view>
#include <vector>

#ifndef NDEBUG
#include <iostream>
#endif


namespace Render {

enum class ShaderType : uint8_t {
	Vertex,
	Fragment
};

template<ShaderType Type>
class Shader {
	unsigned int _id;
public:
	Shader (std::string_view shaderCode) {
		_id = glCreateShader(mapShaderTypeToGLType(Type));
		compileShader(_id, shaderCode.data());
	}

	~Shader() {
		glDeleteShader(_id);
	}

	Shader(Shader& other) = delete;
	Shader(Shader&& other) = delete;
	Shader& operator=(Shader& other) = delete;
	Shader& operator=(Shader&& other) = delete;

	unsigned int Id() const {
		return _id;
	}

	operator unsigned int() const {
		return _id;
	}

private:
	static void compileShader(unsigned int id, const char* shaderCode) {
		glShaderSource(id, 1, &shaderCode, NULL);
		glCompileShader(id);

		#ifndef NDEBUG
		int success;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		#endif
	}

	static int mapShaderTypeToGLType(const ShaderType type) {
		switch(type) {
			case ShaderType::Vertex:
				return GL_VERTEX_SHADER;
			case ShaderType::Fragment:
				return GL_FRAGMENT_SHADER;
			default:
				return -1;
		}
	}
};

class ShaderProgram {
	unsigned int _id = 0;
public:
	template<ShaderType ... Ts>
	ShaderProgram(const Shader<Ts>& ... shaderParts) {
		_id = glCreateProgram();
		(glAttachShader(_id, shaderParts), ...);
		linkProgram();
	}

	ShaderProgram() = default;

	ShaderProgram(ShaderProgram&& other) {
		glDeleteProgram(_id);
		_id = other._id;
		other._id = 0;
	}

	ShaderProgram& operator=(ShaderProgram&& other) {
		glDeleteProgram(_id);
		_id = other._id;
		other._id = 0;
		return *this;
	}

	~ShaderProgram() {
		if (_id != 0) {
			glDeleteProgram(_id);
		}
	}

	template<class T>
	void set(const std::string_view name, const T value) const {
		static_assert("Not implemented for given type.");
	}

	template<class T>
	void setArray(const std::string_view name, const size_t count, const T* value) const {
		static_array("Not implemented for given type.");
	}

	struct ScopedBinding {
        ScopedBinding(const ShaderProgram& shaderProgram) { glUseProgram(shaderProgram._id); }
        ~ScopedBinding() { glUseProgram(0); }
    };

private:
	void linkProgram() {
		glLinkProgram(_id);

		#ifndef NDEBUG
		int success;
		char infoLog[512];
		glGetProgramiv(_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(_id, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
		}
		#endif
	}
};

template<>
void ShaderProgram::set<bool>(const std::string_view name, const bool value) const;

template<>
void ShaderProgram::set<int>(const std::string_view name, const int value) const;

template<>
void ShaderProgram::set<unsigned int>(const std::string_view name, const unsigned int value) const;

template<>
void ShaderProgram::set<float>(const std::string_view name, const float value) const;

template<>
void ShaderProgram::set<glm::mat4>(const std::string_view name, const glm::mat4 value) const;

template<>
void ShaderProgram::set<glm::vec3>(const std::string_view name, const glm::vec3 value) const;

template<>
void ShaderProgram::set<glm::vec2>(const std::string_view name, const glm::vec2 value) const;

template<>
void ShaderProgram::setArray<glm::mat4>(const std::string_view name, const std::size_t count, const glm::mat4* ptr) const;

} // namespace Render