#pragma once

#include "Scene/Scene.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace CombinePass {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

struct SharedData {
    GLuint albedoTexture;
    GLuint diffuseTexture;
    GLuint specularTexture;
    GLuint ditherTexture;
    GLuint contourTexture;
};


class CombineVertexProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    CombineVertexProgram(const CombineVertexProgram& other) = delete;
    CombineVertexProgram& operator=(const CombineVertexProgram& other) = delete;

    GLuint _program = -1;

public:
    CombineVertexProgram();
    ~CombineVertexProgram();

    CombineVertexProgram(CombineVertexProgram&& other);
    CombineVertexProgram& operator=(CombineVertexProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class CombineFragmentProgram {
    // Flags
    //  none.

    // Uniform names
    static constexpr std::string_view AlbedoSamplerUniform = "albedoTexture";
    static constexpr unsigned int AlbedoTextureLocation = 0;
    static constexpr std::string_view DiffuseSamplerUniform = "diffuseTexture";
    static constexpr unsigned int DiffuseTextureLocation = 1;
    static constexpr std::string_view SpecularSamplerUniform = "specularTexture";
    static constexpr unsigned int SpecularTextureLocation = 2;
    static constexpr std::string_view DitheringSamplerUniform = "ditherTexture";
    static constexpr unsigned int DitherTextureLocation = 3;
    static constexpr std::string_view ContourSamplerUniform = "contourTexture";
    static constexpr unsigned int ContourTextureLocation = 4;

    CombineFragmentProgram(const CombineFragmentProgram& other) = delete;
    CombineFragmentProgram& operator=(const CombineFragmentProgram& other) = delete;

    GLuint _program = -1;

public:
    CombineFragmentProgram();
    ~CombineFragmentProgram();

    CombineFragmentProgram(CombineFragmentProgram&& other);
    CombineFragmentProgram& operator=(CombineFragmentProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class CombinePipeline {
    
    struct ScopedBinding {
        ScopedBinding(const CombinePipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    CombinePipeline(const CombinePipeline& other) = delete;
    CombinePipeline& operator=(const CombinePipeline& other) = delete;

    CombineVertexProgram& _vertexProgram;
    CombineFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    CombinePipeline(CombineVertexProgram& vertexProgram, CombineFragmentProgram& fragmentProgram);
    ~CombinePipeline();

    CombinePipeline(CombinePipeline&& other);
    CombinePipeline& operator=(CombinePipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};

struct CombinePipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        // I don't have anything to put here yet.
    };

private:
    std::unordered_map<PropertiesSet, CombinePipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, CombineVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, CombineFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        0;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        0;

    void buildVariant(const PropertiesSet& properties);

public:
    CombinePipelineManager() = default;
    ~CombinePipelineManager() = default;

    const CombinePipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace CombinePass
} // namespace SceneDrawing
} // namespace Renderer