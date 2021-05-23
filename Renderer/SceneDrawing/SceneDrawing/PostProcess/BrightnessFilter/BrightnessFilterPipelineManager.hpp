#pragma once

#include "Scene/Scene.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace PostProcess {
namespace BrightnessFilter {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

struct SharedData {
    GLuint sourceTexture;
    float filterTreshold = 1.0;
};


class BrightnessFilterVertexProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    BrightnessFilterVertexProgram(const BrightnessFilterVertexProgram& other) = delete;
    BrightnessFilterVertexProgram& operator=(const BrightnessFilterVertexProgram& other) = delete;

    GLuint _program = -1;

public:
    BrightnessFilterVertexProgram();
    ~BrightnessFilterVertexProgram();

    BrightnessFilterVertexProgram(BrightnessFilterVertexProgram&& other);
    BrightnessFilterVertexProgram& operator=(BrightnessFilterVertexProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class BrightnessFilterFragmentProgram {
    // Flags
    static constexpr std::string_view NullifyBelowFlag = "#define NULLIFY_BELOW 1\n";

    // Uniform names
    static constexpr std::string_view BrightnessTresholdUniform = "brightnessTreshold";
    static constexpr std::string_view SourceSamplerUniform = "sourceTexture";
    static constexpr unsigned int SourceTextureLocation = 0;

    BrightnessFilterFragmentProgram(const BrightnessFilterFragmentProgram& other) = delete;
    BrightnessFilterFragmentProgram& operator=(const BrightnessFilterFragmentProgram& other) = delete;

    // TODO: Enum
    bool _nullifyBelowTreshold = false;
    GLuint _program = -1;

public:
    BrightnessFilterFragmentProgram(bool nullifyBelowTreshold = false);
    ~BrightnessFilterFragmentProgram();

    BrightnessFilterFragmentProgram(BrightnessFilterFragmentProgram&& other);
    BrightnessFilterFragmentProgram& operator=(BrightnessFilterFragmentProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class BrightnessFilterPipeline {
    
    struct ScopedBinding {
        ScopedBinding(const BrightnessFilterPipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    BrightnessFilterPipeline(const BrightnessFilterPipeline& other) = delete;
    BrightnessFilterPipeline& operator=(const BrightnessFilterPipeline& other) = delete;

    BrightnessFilterVertexProgram& _vertexProgram;
    BrightnessFilterFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    BrightnessFilterPipeline(BrightnessFilterVertexProgram& vertexProgram, BrightnessFilterFragmentProgram& fragmentProgram);
    ~BrightnessFilterPipeline();

    BrightnessFilterPipeline(BrightnessFilterPipeline&& other);
    BrightnessFilterPipeline& operator=(BrightnessFilterPipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};

struct BrightnessFilterPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        NULLIFY_BELOW = 1 << 0
    };

private:
    std::unordered_map<PropertiesSet, BrightnessFilterPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, BrightnessFilterVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, BrightnessFilterFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        0;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        NULLIFY_BELOW;
    void buildVariant(const PropertiesSet& properties);

public:
    BrightnessFilterPipelineManager() = default;
    ~BrightnessFilterPipelineManager() = default;

    const BrightnessFilterPipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace BrightnessFilter
} // namespace PostProcess
} // namespace SceneDrawing
} // namespace Renderer