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


struct SharedData {
    glm::mat4 cameraProjectionTransform{1};
    glm::mat4 cameraViewTransform{1};
};


struct IndividualData {
    int meshId;
    glm::mat4 objectModelTransform{1};
    const Renderer::Scene::Base::Texture* baseColorTexture = nullptr;
    const Renderer::Scene::Base::Texture* normalMapTexture = nullptr;
    const Renderer::Scene::Base::Texture* metallicRoughnessTexture = nullptr;
    const JointsArray* jointsArray = nullptr;
};


class BasePassVertexProgram {
    // Flags
    static constexpr std::string_view SkinFlag = "#define SKINNED_MESH 1\n";
    static constexpr std::string_view NormalMapTextureFlag = "#define NORMAL_MAP_TEXTURE 1\n";
    // Uniform names
    static constexpr std::string_view JointTransformsUniform = "jointTransform";
    static constexpr std::string_view ModelTransformUniform = "model";
    static constexpr std::string_view ViewTransformUniform = "view";
    static constexpr std::string_view ProjectionTransformUniform = "proj";
    static constexpr std::string_view MeshIdUniform = "mesh_id";

    BasePassVertexProgram(const BasePassVertexProgram& other) = delete;
    BasePassVertexProgram& operator=(const BasePassVertexProgram& other) = delete;

    bool _isSkinned = false;
    bool _hasNormalMapTexture = false;
    GLuint _program = -1;

public:
    BasePassVertexProgram(bool skinned, bool hasNormalMapTexture);
    ~BasePassVertexProgram();

    BasePassVertexProgram(BasePassVertexProgram&& other);
    BasePassVertexProgram& operator=(BasePassVertexProgram&& other);

    operator unsigned int() const;

    void prepareShared(const SharedData& sceneView) const;
    void prepareIndividual(const IndividualData& sceneObject) const;
};


class BasePassFragmentProgram {
    // Flags
    static constexpr std::string_view BaseColorTextureFlag = "#define BASE_COLOUR_TEXTURE 1\n";
    static constexpr std::string_view NormalMapTextureFlag = "#define NORMAL_MAP_TEXTURE 1\n";
    static constexpr std::string_view MetallicRoughnessTextureFlag = "#define METALLIC_ROUGHNESS_TEXTURE 1\n";
    // Uniform names
    static constexpr std::string_view BaseColorSamplerUniform = "baseColor";
    static constexpr std::string_view NormalMapSamplerUniform = "normalMap";
    static constexpr std::string_view MetallicRougnessSamplerUniform = "metallicRoughness";
    static constexpr unsigned int BaseColorTextureLocation = 0;
    static constexpr unsigned int NormalMapTextureLocation = 1;
    static constexpr unsigned int MetallicRoughnessTextureLocation = 2;

    BasePassFragmentProgram(const BasePassFragmentProgram& other) = delete;
    BasePassFragmentProgram& operator=(const BasePassFragmentProgram& other) = delete;

    bool _hasBaseColorTexture = false;
    bool _hasNormalMapTexture = false;
    bool _hasMetallicRoughnessTexture = false;
    GLuint _program = -1;

public:
    BasePassFragmentProgram(bool hasBaseColorTexture, bool hasNormalMapTexture, bool hasMetallicRoughnessTexture);
    ~BasePassFragmentProgram();

    BasePassFragmentProgram(BasePassFragmentProgram&& other);
    BasePassFragmentProgram& operator=(BasePassFragmentProgram&& other);

    operator unsigned int() const;

    void prepareShared(const SharedData& sceneView) const;
    void prepareIndividual(const IndividualData& sceneObject) const;
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

    void prepareShared(const SharedData& viewData) const;
    void prepareIndividual(const IndividualData& objectData) const;
};


struct BasePassPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        SKIN = 1 << 0,
        BASE_COLOR_TEXTURE = 1 << 1,
        NORMAL_MAP_TEXTURE = 1 << 2,
        METALLIC_ROUGHNESS_TEXTURE = 1 << 3
    };  

private:
    std::unordered_map<PropertiesSet, BasePassPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, BasePassVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, BasePassFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram = 
        PipelineProperties::SKIN |
        PipelineProperties::NORMAL_MAP_TEXTURE;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        PipelineProperties::BASE_COLOR_TEXTURE |
        PipelineProperties::NORMAL_MAP_TEXTURE |
        PipelineProperties::METALLIC_ROUGHNESS_TEXTURE;

    void buildVariant(const PropertiesSet& properties);

public:
    BasePassPipelineManager() = default;
    ~BasePassPipelineManager() = default;

    const BasePassPipeline& GetPipeline(const PropertiesSet& properties);
};


} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer