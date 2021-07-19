#pragma once

#include "Scene/Scene.hpp"
#include "Base/ShaderProgram.hpp"

#include <string_view>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {
namespace LightingPass {

static constexpr unsigned int MaxDirectionalLightsPerExecute = 4; // HAS TO MATCH SHADER CODE.
static constexpr unsigned int MaxSpotLightsPerExecute = 4; // HAS TO MATCH SHADER CODE.
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

    std::vector<glm::mat4> spotLightsTransforms;
    std::vector<glm::vec4> spotLightsDirections;
    std::vector<glm::vec4> spotLightsPositions;
    std::vector<glm::vec4> spotLightsColors;
    std::vector<glm::vec2> spotLightsInnerOuterConeAngle;
    std::vector<GLuint> spotLightsShadowmapTextureIds;
};


class LightingVertexProgram {
    // Flags
    //  none.
    // Uniform names
    //  none.

    LightingVertexProgram(const LightingVertexProgram& other) = delete;
    LightingVertexProgram& operator=(const LightingVertexProgram& other) = delete;

    Programs::Base::ShaderProgram<Programs::Base::ShaderType::Vertex> _program;

public:
    LightingVertexProgram();

    LightingVertexProgram(LightingVertexProgram&& other);
    LightingVertexProgram& operator=(LightingVertexProgram&& other);

    operator unsigned int() const;
    
    void prepareShared(const SharedData& data) const;
    void prepareIndividual() const;
};


class LightingFragmentProgram {
    // Flags
    static constexpr std::string_view DirectionalLightFlag = "#define DIRECTIONAL_LIGHTS 1\n";
    static constexpr std::string_view SpotLightFlag = "#define SPOT_LIGHTS 1\n";
    static constexpr std::string_view DepthReverseZ = "#define DEPTH_REVERSE_Z 1\n";

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

    static constexpr std::string_view DirectionalLightsCountUniform = "directionalLightCount";
    static constexpr std::string_view DirectionalLightTransformsUniform = "directionalLightTransforms";
    static constexpr std::string_view DirectionalLightDirectionsUniform = "directionalLightDirections";
    static constexpr std::string_view DirectionalLightColorsUniform = "directionalLightColors";
    static constexpr std::string_view DirectionalLightShadowmapSamplersUniform = "directionalLightShadowmapSamplers";
    static constexpr unsigned int DirectionalLightShadowmapTexturesLocationBegin = 4;
    static constexpr unsigned int DirectionalLightShadowmapTexturesLocationEnd = DirectionalLightShadowmapTexturesLocationBegin + MaxDirectionalLightsPerExecute;
    
    static constexpr std::string_view SpotLightsCountUniform = "spotLightCount";
    static constexpr std::string_view SpotLightTransformsUniform = "spotLightTransforms";
    static constexpr std::string_view SpotLightDirectionsUniform = "spotLightDirections";
    static constexpr std::string_view SpotLightPositionsUniform = "spotLightPositions";
    static constexpr std::string_view SpotLightColorsUniform = "spotLightColors";
    static constexpr std::string_view SpotLightInnerOuterConeAnglesUniform = "spotLightInnerOuterConeAngles";
    static constexpr std::string_view SpotLightShadowmapSamplersUniform = "spotLightShadowmapSamplers";
    static constexpr unsigned int SpotLightShadowmapTexturesLocationBegin = DirectionalLightShadowmapTexturesLocationEnd;
    static constexpr unsigned int SpotLightShadowmapTexturesLocationEnd = SpotLightShadowmapTexturesLocationBegin + MaxSpotLightsPerExecute;

    LightingFragmentProgram(const LightingFragmentProgram& other) = delete;
    LightingFragmentProgram& operator=(const LightingFragmentProgram& other) = delete;

    bool _drawDirectionalLight = false;
    bool _drawSpotLight = false;
    Programs::Base::ShaderProgram<Programs::Base::ShaderType::Fragment> _program;

public:
    LightingFragmentProgram(const bool drawDirectionalLight, const bool drawSpotLight);

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
        LIGHTTYPE_DIRECTIONAL = 1 << 0,
        LIGHTTYPE_SPOT = 2 << 0
    };

private:
    std::unordered_map<PropertiesSet, LightingPipeline> _builtPipelines;
    std::unordered_map<PropertiesSet, LightingVertexProgram> _cachedVertexPrograms;
    std::unordered_map<PropertiesSet, LightingFragmentProgram> _cachedFragmentPrograms;

    constexpr static PropertiesSet PropertiesAffectingVertexProgram =
        0;

    constexpr static PropertiesSet PropertiesAffectingFragmentProgram =
        PipelineProperties::LIGHTTYPE_DIRECTIONAL |
        PipelineProperties::LIGHTTYPE_SPOT;

    void buildVariant(const PropertiesSet& properties);

public:
    LightingPipelineManager() = default;
    ~LightingPipelineManager() = default;

    const LightingPipeline& GetPipeline(const PropertiesSet& properties);    
};

} // namespace LightingPass
} // namespace SceneDrawing
} // namespace Renderer