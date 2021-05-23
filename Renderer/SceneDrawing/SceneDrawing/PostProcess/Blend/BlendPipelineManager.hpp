#pragma once

#include "Scene/Scene.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace PostProcess {
namespace Blend {

static constexpr unsigned int MaxBlendSourcesPerExecute = 4; // HAS TO MATCH SHADER CODE.
static constexpr std::string_view VersionFlag = "#version 410 core\n";

struct SharedData {
    std::array<GLuint, MaxBlendSourcesPerExecute> sourceTextures;
    GLuint sourcesCount = 0;
};


class BlendVertexProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    BlendVertexProgram(const BlendVertexProgram& other) = delete;
    BlendVertexProgram& operator=(const BlendVertexProgram& other) = delete;

    GLuint _program = -1;

public:
    BlendVertexProgram();
    ~BlendVertexProgram();

    BlendVertexProgram(BlendVertexProgram&& other);
    BlendVertexProgram& operator=(BlendVertexProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class BlendFragmentProgram {
    // Flags
    static constexpr std::string_view AdditiveBlendFlag = "#define ADDITIVE 1\n";

    // Uniform names
    static constexpr std::string_view SourcesCountUniform = "sourcesCount";
    static constexpr std::string_view SourceSamplersUniform = "sourceSamplers";
    static constexpr unsigned int SourceTextureLocationBegin = 0;
    static constexpr unsigned int SourceTextureLocationEnd = SourceTextureLocationBegin + MaxBlendSourcesPerExecute;

    BlendFragmentProgram(const BlendFragmentProgram& other) = delete;
    BlendFragmentProgram& operator=(const BlendFragmentProgram& other) = delete;

    // TODO: Enum
    bool _additive = false;
    GLuint _program = -1;

public:
    BlendFragmentProgram(bool additive);
    ~BlendFragmentProgram();

    BlendFragmentProgram(BlendFragmentProgram&& other);
    BlendFragmentProgram& operator=(BlendFragmentProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class BlendPipeline {
    
    struct ScopedBinding {
        ScopedBinding(const BlendPipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    BlendPipeline(const BlendPipeline& other) = delete;
    BlendPipeline& operator=(const BlendPipeline& other) = delete;

    BlendVertexProgram& _vertexProgram;
    BlendFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    BlendPipeline(BlendVertexProgram& vertexProgram, BlendFragmentProgram& fragmentProgram);
    ~BlendPipeline();

    BlendPipeline(BlendPipeline&& other);
    BlendPipeline& operator=(BlendPipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};

struct BlendPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        ADDITIVE = 1 << 0
    };

private:
    std::unordered_map<PropertiesSet, BlendPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, BlendVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, BlendFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        0;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        ADDITIVE;
    void buildVariant(const PropertiesSet& properties);

public:
    BlendPipelineManager() = default;
    ~BlendPipelineManager() = default;

    const BlendPipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace Blend
} // namespace PostProcess
} // namespace SceneDrawing
} // namespace Renderer