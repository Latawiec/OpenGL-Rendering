#pragma once

#include "Material.hpp"

#include <unordered_map>

#ifndef NDEBUG
#include <iostream>
#endif

#ifndef BASE_MATERIAL_VERTEX_SOURCE_PATH
#define BASE_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef BASE_MATERIAL_FRAGMENT_SOURCE_PATH
#define BASE_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 

namespace Render {
namespace Common {

enum class ProgramType : uint8_t {
    Vertex,
    Fragment
};

template<ProgramType Stage>
struct Program {

    Program(const std::string_view sourcePath, const Material::PropertiesSet properties) {

        std::vector<const char*> compilerInput {};
        std::string shaderSource;
        GLuint shaderType;

        if (Stage == ProgramType::Fragment) {
            if (properties & static_cast<Material::PropertiesSet>(Material::Properties::SKIN)) {
                compilerInput.emplace_back("#define SKINNED_MESH 1");
            }

            shaderSource = Utils::readFile(BASE_MATERIAL_VERTEX_SOURCE_PATH);
            shaderType = GL_VERTEX_SHADER;
        }

        if constexpr (Stage == ProgramType::Vertex) {
            if (properties & static_cast<Material::PropertiesSet>(Material::Properties::SHADOW_MAPPED)) {
                compilerInput.emplace_back("#define SHADOW_MAPPED 1");
            }

            if (properties & static_cast<Material::PropertiesSet>(Material::Properties::BASE_COLOUR_TEXTURE)) {
                compilerInput.emplace_back("#define BASE_COLOUR_TEXTURE 1");
            }

            if (properties & static_cast<Material::PropertiesSet>(Material::Properties::NORMAL_MAP_TEXTURE)) {
                compilerInput.emplace_back("#define NORMAL_MAP_TEXTURE 1");
            }

            shaderSource = Utils::readFile(BASE_MATERIAL_FRAGMENT_SOURCE_PATH);
            compilerInput.emplace_back(shaderSource.data());
            shaderType = GL_FRAGMENT_SHADER;
        }


        // Compile program.
        _program = glCreateProgram();
        glProgramParameteri(_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

        GLuint shader  = glCreateShader(shaderType);
        glShaderSource(shader, compilerInput.size(), compilerInput.data(), NULL);
        glCompileShader(shader);

    #ifndef NDEBUG
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    #endif

        glAttachShader(_program, shader);
        glLinkProgram(_program);

        // Delete shader as we only need program.
        glDetachShader(_program, shader);
        glDeleteShader(shader);     
    }

    ~Program() {
        if (_program != -1) {
            glDeleteProgram(_program);
        }
    }

    Program(Program&& other) {
        const auto hold = _program;
        this->_program = other._program;
        other._program = hold;
    }

    Program& operator=(Program&& other) {
        const auto hold = _program;
        this->_program = other._program;
        other._program = hold;

        return *this;
    }

    operator unsigned int() const {
        return _program;
    }

private:
    Program(const Program& other) = delete;
    Program& operator=(const Program& other) = delete;
    GLuint _program = -1;
};

struct Pipeline {
private:
    struct ScopedBinding {
        ScopedBinding(const Pipeline& pipeline) { glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };
public:

    Pipeline(const Program<ProgramType::Vertex>& vertexProgram, const Program<ProgramType::Fragment>& fragmentProgram) {
        glGenProgramPipelines(1, &_pipeline);

        glUseProgramStages(_pipeline, GL_VERTEX_SHADER_BIT, vertexProgram);
        glUseProgramStages(_pipeline, GL_FRAGMENT_SHADER_BIT, fragmentProgram);
    }

    ~Pipeline() {
        if (_pipeline != -1) {
            glDeleteProgramPipelines(1, &_pipeline);
        }
    }

    Pipeline(Pipeline&& other) {
        const auto hold = _pipeline;
        this->_pipeline = other._pipeline;
        other._pipeline = hold;
    }

    Pipeline& operator=(Pipeline&& other) {
        const auto hold = _pipeline;
        this->_pipeline = other._pipeline;
        other._pipeline = hold;

        return *this;
    }

    operator unsigned int() const {
        return _pipeline;
    }

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const {
        return {*this};
    }

private:

    Pipeline(const Pipeline& other) = delete;
    Pipeline& operator=(const Pipeline& other) = delete;
    GLuint _pipeline;
};

struct MaterialManager {

    MaterialManager() = default;

    const Pipeline& GetPipeline(const Material::PropertiesSet properties) {
        if (_pipelinesCache.contains(properties)) {
            return _pipelinesCache[properties];
        } else {
            return compileVariant(properties);
        }
    }

private:

    Pipeline& compileVariant(const Material::PropertiesSet properties) {
        const auto isCorrectVertexVariantCompiled = _vertexStagePrograms.contains(properties & VertexShaderVariantsMask);
        if (!isCorrectVertexVariantCompiled) {
            const auto variantFlags = properties & VertexShaderVariantsMask;
            _vertexStagePrograms.emplace(variantFlags, {BASE_MATERIAL_VERTEX_SOURCE_PATH, variantFlags});
        }
        const auto& vertexProgram = _vertexStagePrograms[properties & VertexShaderVariantsMask];

        const auto isCorrectFragmentVariantCompiled = _fragmentStagePrograms.contains(properties & FragmentShaderVariantsMask);
        if (!isCorrectFragmentVariantCompiled) {
            const auto variantFlags = properties & FragmentShaderVariantsMask;
            _fragmentStagePrograms.emplace(variantFlags, {BASE_MATERIAL_FRAGMENT_SOURCE_PATH, variantFlags});
        }
        const auto& fragmentProgram = _fragmentStagePrograms[properties & FragmentShaderVariantsMask];

        return *_pipelinesCache.emplace(properties, {vertexProgram, fragmentProgram});
    }

    std::unordered_map<Material::PropertiesSet, Pipeline> _pipelinesCache;
    std::unordered_map<Material::PropertiesSet, Program<ProgramType::Vertex>> _vertexStagePrograms;
    std::unordered_map<Material::PropertiesSet, Program<ProgramType::Fragment>> _fragmentStagePrograms;

    std::string_view VertexShaderSourcePath = BASE_MATERIAL_VERTEX_SOURCE_PATH;
    std::string_view FragmentShaderSourcePath = BASE_MATERIAL_FRAGMENT_SOURCE_PATH;

    const Material::PropertiesSet VertexShaderVariantsMask =
        static_cast<Material::PropertiesSet>(Material::Properties::SKIN);

    const Material::PropertiesSet FragmentShaderVariantsMask =
        static_cast<Material::PropertiesSet>(Material::Properties::SHADOW_MAPPED) |
        static_cast<Material::PropertiesSet>(Material::Properties::BASE_COLOUR_TEXTURE) |
        static_cast<Material::PropertiesSet>(Material::Properties::NORMAL_MAP_TEXTURE);
};

}
}