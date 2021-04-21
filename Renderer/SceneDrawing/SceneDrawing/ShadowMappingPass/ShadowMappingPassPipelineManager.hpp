#pragma once

#include "Scene/Scene.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace ShadowMappingPass {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

static constexpr int MaxJointsCount = 32;
using JointsArray = std::array<glm::mat4, MaxJointsCount>;

struct SceneViewData {
    glm::mat4 lightProjectionTransform {1};
    glm::mat4 lightViewTransform {1};
};

struct SceneObjectData {
    glm::mat4 objectModelTransform {1};
    const JointsArray* jointsArray = nullptr;
};

enum class LightType : uint8_t {
    NONE,
    DIRECTIONAL
};


class ShadowMappingVertexProgram {
    // Flags
    static constexpr std::string_view DirectionalLightFlag = "#define DIRECTIONAL_LIGHT 1\n";
    static constexpr std::string_view SkinFlag = "#define SKINNED_MESH 1\n";
    // Uniform names
    static constexpr std::string_view JointTransformsUniform = "jointTransform";
    static constexpr std::string_view ModelTransformUniform = "model";
    static constexpr std::string_view ViewTransformUniform = "view";
    static constexpr std::string_view ProjectionTransformUniform = "proj";

    ShadowMappingVertexProgram(const ShadowMappingVertexProgram& other) = delete;
    ShadowMappingVertexProgram& operator=(const ShadowMappingVertexProgram& other) = delete;

    LightType _type;
    bool _isSkinned = false;
    GLuint _program = -1;

public:
    ShadowMappingVertexProgram(const LightType type, bool skinned);
    ~ShadowMappingVertexProgram();

    ShadowMappingVertexProgram(ShadowMappingVertexProgram&& other);
    ShadowMappingVertexProgram& operator=(ShadowMappingVertexProgram&& other);

    operator unsigned int() const;
    
    void PrepareView(const SceneViewData& sceneView) const;
    void PrepareElement(const SceneObjectData& sceneObject) const;
};


class ShadowMappingFragmentProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    ShadowMappingFragmentProgram(const ShadowMappingFragmentProgram& other) = delete;
    ShadowMappingFragmentProgram& operator=(const ShadowMappingFragmentProgram& other) = delete;

    GLuint _program = -1;

public:
    ShadowMappingFragmentProgram();
    ~ShadowMappingFragmentProgram();

    ShadowMappingFragmentProgram(ShadowMappingFragmentProgram&& other);
    ShadowMappingFragmentProgram& operator=(ShadowMappingFragmentProgram&& other);

    operator unsigned int() const;
    
    void PrepareView(const SceneViewData& sceneView) const;
    void PrepareElement(const SceneObjectData& sceneObject) const;
};


class ShadowMappingPipeline {
    
    struct ScopedBinding {
        ScopedBinding(const ShadowMappingPipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    ShadowMappingPipeline(const ShadowMappingPipeline& other) = delete;
    ShadowMappingPipeline& operator=(const ShadowMappingPipeline& other) = delete;

    ShadowMappingVertexProgram& _vertexProgram;
    ShadowMappingFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    ShadowMappingPipeline(ShadowMappingVertexProgram& vertexProgram, ShadowMappingFragmentProgram& fragmentProgram);
    ~ShadowMappingPipeline();

    ShadowMappingPipeline(ShadowMappingPipeline&& other);
    ShadowMappingPipeline& operator=(ShadowMappingPipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void PrepareView(const SceneViewData& sceneView) const;
    void PrepareElement(const SceneObjectData& sceneObject) const;
};

struct ShadowMappingPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        SKIN = 1 << 0,
        LIGHTTYPE_DIRECTIONAL = 1 << 1
    };

private:
    std::unordered_map<PropertiesSet, ShadowMappingPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, ShadowMappingVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, ShadowMappingFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        PipelineProperties::SKIN |
        PipelineProperties::LIGHTTYPE_DIRECTIONAL;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        0;

    void buildVariant(const PropertiesSet& properties);

public:
    ShadowMappingPipelineManager() = default;
    ~ShadowMappingPipelineManager() = default;

    const ShadowMappingPipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace ShadowMapping
} // namespace SceneDrawing
} // namespace Renderer