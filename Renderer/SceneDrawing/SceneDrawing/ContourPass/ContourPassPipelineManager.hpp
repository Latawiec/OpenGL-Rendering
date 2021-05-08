#pragma once

#include "Scene/Scene.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace ContourPass {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

struct SharedData {
    GLuint silhouetteTexture;
};


class ContourVertexProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    ContourVertexProgram(const ContourVertexProgram& other) = delete;
    ContourVertexProgram& operator=(const ContourVertexProgram& other) = delete;

    GLuint _program = -1;

public:
    ContourVertexProgram();
    ~ContourVertexProgram();

    ContourVertexProgram(ContourVertexProgram&& other);
    ContourVertexProgram& operator=(ContourVertexProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class ContourFragmentProgram {
    // Flags
    //  none.

    // Uniform names
    static constexpr std::string_view SilhouetteSamplerUniform = "silhouetteTexture";
    static constexpr unsigned int SilhouetteTextureLocation = 0;

    ContourFragmentProgram(const ContourFragmentProgram& other) = delete;
    ContourFragmentProgram& operator=(const ContourFragmentProgram& other) = delete;

    GLuint _program = -1;

public:
    ContourFragmentProgram();
    ~ContourFragmentProgram();

    ContourFragmentProgram(ContourFragmentProgram&& other);
    ContourFragmentProgram& operator=(ContourFragmentProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class ContourPipeline {
    
    struct ScopedBinding {
        ScopedBinding(const ContourPipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    ContourPipeline(const ContourPipeline& other) = delete;
    ContourPipeline& operator=(const ContourPipeline& other) = delete;

    ContourVertexProgram& _vertexProgram;
    ContourFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    ContourPipeline(ContourVertexProgram& vertexProgram, ContourFragmentProgram& fragmentProgram);
    ~ContourPipeline();

    ContourPipeline(ContourPipeline&& other);
    ContourPipeline& operator=(ContourPipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};

struct ContourPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        // I don't have anything to put here yet.
    };

private:
    std::unordered_map<PropertiesSet, ContourPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, ContourVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, ContourFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        0;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        0;

    void buildVariant(const PropertiesSet& properties);

public:
    ContourPipelineManager() = default;
    ~ContourPipelineManager() = default;

    const ContourPipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace ContourPass
} // namespace SceneDrawing
} // namespace Renderer