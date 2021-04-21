#pragma once

#include "Scene/Scene.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace BasePass {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

static constexpr int MaxJointsCount = 32;
using JointsArray = std::array<glm::mat4, MaxJointsCount>;


struct SceneViewData {
    glm::mat4 cameraProjectionTransform{1};
    glm::mat4 cameraViewTransform{1};
};


struct SceneObjectData {
    int meshId;
    glm::mat4 objectModelTransform{1};
    const Renderer::Scene::Base::Texture* baseColorTexture = nullptr;
    const Renderer::Scene::Base::Texture* normalMapTexture = nullptr;
    const JointsArray* jointsArray = nullptr;
};


class BasePassVertexProgram {
    // Flags
    static constexpr std::string_view SkinFlag = "#define SKINNED_MESH 1\n";
    // Uniform names
    static constexpr std::string_view JointTransformsUniform = "jointTransform";
    static constexpr std::string_view ModelTransformUniform = "model";
    static constexpr std::string_view ViewTransformUniform = "view";
    static constexpr std::string_view ProjectionTransformUniform = "proj";
    static constexpr std::string_view MeshIdUniform = "mesh_id";

    BasePassVertexProgram(const BasePassVertexProgram& other) = delete;
    BasePassVertexProgram& operator=(const BasePassVertexProgram& other) = delete;

    bool _isSkinned = false;
    GLuint _program = -1;

public:
    BasePassVertexProgram(bool skinned);
    ~BasePassVertexProgram();

    BasePassVertexProgram(BasePassVertexProgram&& other);
    BasePassVertexProgram& operator=(BasePassVertexProgram&& other);

    operator unsigned int() const;

    void PrepareView(const SceneViewData& sceneView) const;
    void PrepareElement(const SceneObjectData& sceneObject) const;
};


class BasePassFragmentProgram {
    // Flags
    static constexpr std::string_view BaseColorTextureFlag = "#define BASE_COLOUR_TEXTURE 1\n";
    static constexpr std::string_view NormalMapTextureFlag = "#define NORMAL_MAP_TEXTURE 1\n";
    // Uniform names
    static constexpr std::string_view BaseColorSamplerUniform = "baseColor";
    static constexpr std::string_view NormalMapSamplerUniform = "normalMap";
    static constexpr unsigned int BaseColorTextureLocation = 0;
    static constexpr unsigned int NormalMapTextureLocation = 1;

    BasePassFragmentProgram(const BasePassFragmentProgram& other) = delete;
    BasePassFragmentProgram& operator=(const BasePassFragmentProgram& other) = delete;

    bool _hasBaseColorTexture = false;
    bool _hasNormalMapTexture = false;
    GLuint _program = -1;

public:
    BasePassFragmentProgram(bool hasBaseColorTexture, bool hasNormalMapTexture);
    ~BasePassFragmentProgram();

    BasePassFragmentProgram(BasePassFragmentProgram&& other);
    BasePassFragmentProgram& operator=(BasePassFragmentProgram&& other);

    operator unsigned int() const;

    void PrepareView(const SceneViewData& sceneView) const;
    void PrepareElement(const SceneObjectData& sceneObject) const;
};


class BasePassPipeline {

    struct ScopedBinding {
        ScopedBinding(const BasePassPipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    BasePassPipeline(const BasePassPipeline& other) = delete;
    BasePassPipeline& operator=(const BasePassPipeline& other) = delete;

    BasePassVertexProgram& _vertexProgram;
    BasePassFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    BasePassPipeline(BasePassVertexProgram& vertexProgram, BasePassFragmentProgram& fragmentProgram);
    ~BasePassPipeline();

    BasePassPipeline(BasePassPipeline&& other);
    BasePassPipeline& operator=(BasePassPipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void PrepareView(const SceneViewData& viewData) const;
    void PrepareElement(const SceneObjectData& objectData) const;
};


struct BasePassPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        SKIN = 1 << 0,
        BASE_COLOR_TEXTURE = 1 << 1,
        NORMAL_MAP_TEXTURE = 1 << 2
    };  

private:
    std::unordered_map<PropertiesSet, BasePassPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, BasePassVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, BasePassFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram = 
        PipelineProperties::SKIN;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        PipelineProperties::BASE_COLOR_TEXTURE |
        PipelineProperties::NORMAL_MAP_TEXTURE;

    void buildVariant(const PropertiesSet& properties);

public:
    BasePassPipelineManager() = default;
    ~BasePassPipelineManager() = default;

    const BasePassPipeline& GetPipeline(const PropertiesSet& properties);
};


} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer