#pragma once

#include "Scene/Scene.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace LightingPass {

static constexpr unsigned int MaxDirectionalLightsPerExecute = 4; // HAS TO MATCH SHADER CODE.
static constexpr std::string_view VersionFlag = "#version 410 core\n";

struct SharedData {
    GLuint albedoTexture;
    GLuint positionTexture;
    GLuint normalMapTexture;
    GLuint metallicRoughnessTexture;

    glm::vec4 cameraPosition;

    std::vector<glm::mat4> directionalLightsTransforms;
    std::vector<glm::vec4> directionalLightsDirections;
    std::vector<glm::vec4> directionalLightsColors;
    std::vector<GLuint> directionalLightsShadowmapTextureIds;
};


enum class LightType : uint8_t {
    NONE,
    DIRECTIONAL
};

class LightingVertexProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    LightingVertexProgram(const LightingVertexProgram& other) = delete;
    LightingVertexProgram& operator=(const LightingVertexProgram& other) = delete;

    GLuint _program = -1;

public:
    LightingVertexProgram();
    ~LightingVertexProgram();

    LightingVertexProgram(LightingVertexProgram&& other);
    LightingVertexProgram& operator=(LightingVertexProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class LightingFragmentProgram {
    // Flags
    static constexpr std::string_view DirectionalLightFlag = "#define DIRECTIONAL_LIGHTS 1\n";

    // Uniform names
    static constexpr std::string_view AlbedoSamplerUniform = "albedoTexture";
    static constexpr unsigned int AlbedoTextureLocation = 0;
    static constexpr std::string_view PositionSamplerUniform = "positionTexture";
    static constexpr unsigned int PositionTextureLocation = 1;
    static constexpr std::string_view NormalMapSamplerUniform = "normalMapTexture";
    static constexpr unsigned int NormalMapTextureLocation = 2;
    static constexpr std::string_view MetallicRoughnessSamplerUniform = "metallicRoughnessTexture";
    static constexpr unsigned int MetallicRoughnessTextureLocation = 3;

    static constexpr std::string_view CameraPositionUniform = "cameraPosition";

    static constexpr std::string_view DirectionalLightsCountUniform = "directionalLightsCount";
    static constexpr std::string_view DirectionalLightTransformsUniform = "directionalLightTransforms";
    static constexpr std::string_view DirectionalLightDirectionsUniform = "directionalLightDirections";
    static constexpr std::string_view DirectionalLightColor = "directionalLightColor";
    static constexpr std::string_view DirectionalLightShadowmapSamplersUniform = "directionalLightShadowmapSamplers";
    static constexpr unsigned int DirectionalLightShadowmapTexturesLocationBegin = 4;
    static constexpr unsigned int DirectionalLightShadowmapTexturesLocationEnd = DirectionalLightShadowmapTexturesLocationBegin + MaxDirectionalLightsPerExecute;

    LightingFragmentProgram(const LightingFragmentProgram& other) = delete;
    LightingFragmentProgram& operator=(const LightingFragmentProgram& other) = delete;

    LightType _type;
    GLuint _program = -1;

public:
    LightingFragmentProgram(const LightType type);
    ~LightingFragmentProgram();

    LightingFragmentProgram(LightingFragmentProgram&& other);
    LightingFragmentProgram& operator=(LightingFragmentProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class LightingPipeline {
    
    struct ScopedBinding {
        ScopedBinding(const LightingPipeline& pipeline) { glUseProgram(0); glBindProgramPipeline(pipeline._pipeline); }
        ~ScopedBinding() { glBindProgramPipeline(0); }
    };

    LightingPipeline(const LightingPipeline& other) = delete;
    LightingPipeline& operator=(const LightingPipeline& other) = delete;

    LightingVertexProgram& _vertexProgram;
    LightingFragmentProgram& _fragmentProgram;
    GLuint _pipeline = -1;

public:
    LightingPipeline(LightingVertexProgram& vertexProgram, LightingFragmentProgram& fragmentProgram);
    ~LightingPipeline();

    LightingPipeline(LightingPipeline&& other);
    LightingPipeline& operator=(LightingPipeline&& other);

    operator unsigned int() const;

    [[nodiscard("RAII variable discarded.")]]
    ScopedBinding Bind() const;

    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};

struct LightingPipelineManager {
    using PropertiesSet = uint64_t;
    enum PipelineProperties : PropertiesSet {
        LIGHTTYPE_DIRECTIONAL = 1 << 0
    };

private:
    std::unordered_map<PropertiesSet, LightingPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, LightingVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, LightingFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        0;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        PipelineProperties::LIGHTTYPE_DIRECTIONAL;

    void buildVariant(const PropertiesSet& properties);

public:
    LightingPipelineManager() = default;
    ~LightingPipelineManager() = default;

    const LightingPipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace LightingPass
} // namespace SceneDrawing
} // namespace Renderer