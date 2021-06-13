#include "../ShaderCompiler/ShaderCompiler.hpp"

#include <read_file.hpp>
#ifndef NDEBUG
#include <iostream>
#endif

namespace Renderer {
namespace Programs {
namespace ShaderCompiler {

ShaderData::ShaderData(const ShaderType type, const std::string& sourceFilePath)
: _type(type)
, _shaderCode(Utils::readFile(sourceFilePath))
{}

void ShaderData::AddFlag(const std::string& flag) {
    _dynamicFlags.push_back(flag);
}

void ShaderData::AddFlag(const std::string_view& flag) {
    _staticFlags.push_back(flag);
}

ShaderType ShaderData::GetType() const {
    return _type;
}

const std::vector<std::string_view>& ShaderData::GetStaticFlags() const {
    return _staticFlags;
}

const std::vector<std::string>& ShaderData::GetDynamicFlags() const {
    return _dynamicFlags;
}

const std::string& ShaderData::GetShaderCode() const {
    return _shaderCode;
}

GLuint Compile(const ShaderData& data) {
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
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION: Failed\n" << infoLog << std::endl;
    }
}
#endif

    glDetachShader(outputProgram, shader);
    glDeleteShader(shader);

    return outputProgram;
}

} // namespace ShaderCompiler
} // namespace Programs
} // namespace Renderer