#pragma once

#include <glad/glad.h>
#include <string_view>
#include <vector>
#include <string>

#include <read_file.hpp>
#include "ShaderType.hpp"
#include "ShaderProgram.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

namespace Renderer {
namespace Programs {
namespace Base {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

template<ShaderType Type>
struct ShaderData {

    ShaderData(const std::string& sourceFilePath)
    : _shaderCode(Utils::readFile(sourceFilePath)) {}

    void AddFlag(const std::string& flag) {
        _dynamicFlags.push_back(flag);
    }

    void AddFlag(const std::string_view& flag) {
        _staticFlags.push_back(flag);
    }

    ShaderType GetType() const {
        return Type;
    }

    const std::vector<std::string_view>& GetStaticFlags() const {
        return _staticFlags;
    }

    const std::vector<std::string>& GetDynamicFlags() const {
        return _dynamicFlags;
    }

    const std::string& GetShaderCode() const {
        return _shaderCode;
    }

private:
    std::vector<std::string_view> _staticFlags;
    std::vector<std::string> _dynamicFlags;
    std::string _shaderCode;
};

template<ShaderType Type>
ShaderProgram<Type> Compile(const ShaderData<Type>& data) {
    std::vector<const char*> compilerInput = { VersionFlag.data() };

    for (const auto& flag : data.GetStaticFlags()) {
        compilerInput.push_back(flag.data());
    }

    for (const auto& flag : data.GetDynamicFlags()) {
        compilerInput.push_back(flag.data());
    }

    compilerInput.push_back(data.GetShaderCode().data());

    GLuint outputProgram = glCreateProgram();
    glProgramParameteri(outputProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

    GLuint shaderTypeFlag;
    switch (data.GetType()) {
        case ShaderType::Vertex:
            shaderTypeFlag = GL_VERTEX_SHADER;
            break;
        case ShaderType::Fragment:
            shaderTypeFlag = GL_FRAGMENT_SHADER;
            break;
        case ShaderType::Geometry:
            shaderTypeFlag = GL_GEOMETRY_SHADER;
            break;
    }

    GLuint shader = glCreateShader(shaderTypeFlag);
    glShaderSource(shader, compilerInput.size(), compilerInput.data(), NULL);
    glCompileShader(shader);

#ifndef NDEBUG
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION: Failed\n" << infoLog << std::endl;
        throw std::runtime_error("Failed to compile shader.");
    }
}
#endif

    glAttachShader(outputProgram, shader);
    glLinkProgram(outputProgram);

#ifndef NDEBUG
{
    int success;
    char infoLog[512];
    glGetProgramiv(outputProgram, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        glGetProgramInfoLog(outputProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINK: Failed\n" << infoLog << std::endl;
        throw std::runtime_error("Failed to link shader.");
    }
}
#endif

    glDetachShader(outputProgram, shader);
    glDeleteShader(shader);

    return { outputProgram };
}

} // namespace Base
} // namespace Programs
} // namespace Renderer