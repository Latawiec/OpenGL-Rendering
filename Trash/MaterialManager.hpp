#pragma once

#include <Scene/Material.hpp>
#include "read_file.hpp"

#include <unordered_map>

#ifndef NDEBUG
#include <iostream>
#endif


namespace SceneDrawing {

enum class ProgramType : uint8_t {
    Vertex,
    Fragment
};

template<ProgramType Stage>
struct Program {

    Program(const std::string_view sourcePath, const Material::PropertiesSet properties) {

        std::vector<const char*> compilerInput {};
        GLuint shaderType;

        if (Stage == ProgramType::Fragment) {
            if (properties & static_cast<Material::PropertiesSet>(Material::Properties::SKIN)) {
                compilerInput.emplace_back("#define SKINNED_MESH 1");
            }

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

            shaderType = GL_FRAGMENT_SHADER;
        }
        
        const std::string shaderSource = Utils::readFile(sourcePath);
        compilerInput.emplace_back(shaderSource.data());

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
    Pipeline(const Program<ProgramType::Vertex>& vertexProgram, const Program<ProgramType::Fragment>& fragmentProgram);
    ~Pipeline();

    Pipeline(Pipeline&& other);
    Pipeline& operator=(Pipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

private:
    Pipeline(const Pipeline& other) = delete;
    Pipeline& operator=(const Pipeline& other) = delete;
    const Program<ProgramType::Vertex>& vertexProgram;
    const Program<ProgramType::Fragment>& fragmentProgram;
    GLuint _pipeline;
};

struct MaterialManager {

    using PropertiesSet = uint64_t;

    enum class Properties : uint64_t { 
        // Vertex variants
        SKIN = 1 << 0,

        // Fragment variants
        SHADOW_MAPPED = 1 << 1,
        BASE_COLOUR_TEXTURE = 1 << 2,
        NORMAL_MAP_TEXTURE = 1 << 3
    };

    MaterialManager() = default;
    ~MaterialManager();

    Pipeline& PreparePipeline(const Common::Scene::SceneObject& sceneObject, const Common::Scene::SceneView& view, const Common::Scene& scene) const;

private:
    Pipeline& compileVariant(const PropertiesSet properties);

    std::unordered_map<PropertiesSet, Pipeline> _pipelinesCache = {};
    std::unordered_map<PropertiesSet, Program<ProgramType::Vertex>> _vertexStagePrograms = {};
    std::unordered_map<PropertiesSet, Program<ProgramType::Fragment>> _fragmentStagePrograms = {};

    constexpr static PropertiesSet VertexShaderVariantsMask =
        static_cast<PropertiesSet>(Properties::SKIN);

    constexpr static PropertiesSet FragmentShaderVariantsMask =
        static_cast<PropertiesSet>(Properties::SHADOW_MAPPED) |
        static_cast<PropertiesSet>(Properties::BASE_COLOUR_TEXTURE) |
        static_cast<PropertiesSet>(Properties::NORMAL_MAP_TEXTURE);
};

}