#pragma once

#include "Scene/Scene.hpp"
#include "Base/ShaderProgram.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace PostProcess {
namespace Blur {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

struct SharedData {
    GLuint sourceTexture;
    float blurAmount = 1.f;
};


class BlurVertexProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    BlurVertexProgram(const BlurVertexProgram& other) = delete;
    BlurVertexProgram& operator=(const BlurVertexProgram& other) = delete;

    Programs::Base::ShaderProgram<Programs::Base::ShaderType::Vertex> _program;

public:
    BlurVertexProgram();

    BlurVertexProgram(BlurVertexProgram&& other);
    BlurVertexProgram& operator=(BlurVertexProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class BlurFragmentProgram {
    // Flags
    static constexpr std::string_view VerticalBlurFlag = "#define VERTICAL 1\n";
    static constexpr std::string_view HorizontalBlurFlag = "#define HORIZONTAL 1\n";

    // Uniform names
    static constexpr std::string_view SourceSamplerUniform = "sourceTexture";
    static constexpr unsigned int SourceTextureLocation = 0;

    BlurFragmentProgram(const BlurFragmentProgram& other) = delete;
    BlurFragmentProgram& operator=(const BlurFragmentProgram& other) = delete;

    // TODO: Enum
    // It has to be either horizontal or vertical. We doing Gaussian blur.
    bool _isVertical = false;
    Programs::Base::ShaderProgram<Programs::Base::ShaderType::Fragment> _program;

public:
    BlurFragmentProgram(bool isVertical = false);

    BlurFragmentProgram(BlurFragmentProgram&& other);
    BlurFragmentProgram& operator=(BlurFragmentProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class BlurPipeline {
    
    struct ScopedBinding {
        ScopedBinding(const BlurPipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    BlurPipeline(const BlurPipeline& other) = delete;
    BlurPipeline& operator=(const BlurPipeline& other) = delete;

    BlurVertexProgram& _vertexProgram;
    BlurFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    BlurPipeline(BlurVertexProgram& vertexProgram, BlurFragmentProgram& fragmentProgram);
    ~BlurPipeline();

    BlurPipeline(BlurPipeline&& other);
    BlurPipeline& operator=(BlurPipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};

struct BlurPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        VERTICAL_BLUR = 1 << 0,
        HORIZONTAL_BLUR = 1 << 1
    };

private:
    std::unordered_map<PropertiesSet, BlurPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, BlurVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, BlurFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        0;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        VERTICAL_BLUR |
        HORIZONTAL_BLUR;

    void buildVariant(const PropertiesSet& properties);

public:
    BlurPipelineManager() = default;
    ~BlurPipelineManager() = default;

    const BlurPipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace Blur
} // namespace PostProcess
} // namespace SceneDrawing
} // namespace Renderer