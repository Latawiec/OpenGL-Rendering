#include "SceneDrawing/LightingPass/LightingPassPipelineManager.hpp"
#include "Base/ShaderCompiler.hpp"
#include "Base/UniformValue.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <numeric>
#include <sstream>

#ifndef LIGHTING_MATERIAL_VERTEX_SOURCE_PATH
#define LIGHTING_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef LIGHTING_MATERIAL_FRAGMENT_SOURCE_PATH
#define LIGHTING_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace LightingPass {

LightingVertexProgram::LightingVertexProgram()
{
    Programs::Base::ShaderData<Programs::Base::ShaderType::Vertex> data(LIGHTING_MATERIAL_VERTEX_SOURCE_PATH);
    _program = Programs::Base::Compile(data);
}

LightingVertexProgram::LightingVertexProgram(LightingVertexProgram&& other) {
    std::swap(other._program, this->_program);
}

LightingVertexProgram& LightingVertexProgram::operator=(LightingVertexProgram&& other) {
    std::swap(other._program, this->_program);

    return *this;
}

LightingVertexProgram::operator unsigned int() const {
    return _program;
}

void LightingVertexProgram::prepareShared(const SharedData& data) const {
    
}

void LightingVertexProgram::prepareIndividual() const {
    
}


LightingFragmentProgram::LightingFragmentProgram(const bool drawDirectionalLight, const bool drawSpotLight)
: _drawDirectionalLight(drawDirectionalLight)
, _drawSpotLight(drawSpotLight)
{
    Programs::Base::ShaderData<Programs::Base::ShaderType::Fragment> data(LIGHTING_MATERIAL_FRAGMENT_SOURCE_PATH);
    if (_drawDirectionalLight) {
        data.AddFlag(DirectionalLightFlag);
    }

    if (_drawSpotLight) {
        data.AddFlag(SpotLightFlag);
    }

    _program = Programs::Base::Compile(data);

    // Setup textures
    using namespace Programs::Base;
    UniformValue<UniformType::Sampler2D>(_program, AlbedoSamplerUniform).Set(AlbedoTextureLocation);
    UniformValue<UniformType::Sampler2D>(_program, PositionSamplerUniform).Set(PositionTextureLocation);
    UniformValue<UniformType::Sampler2D>(_program, NormalMapSamplerUniform).Set(NormalMapTextureLocation);
    UniformValue<UniformType::Sampler2D>(_program, MetallicRoughnessSamplerUniform).Set(MetallicRoughnessTextureLocation);

    std::array<GLint, MaxDirectionalLightsPerExecute> directionalLightShadowmapTexturesLocations;
    std::iota(directionalLightShadowmapTexturesLocations.begin(), directionalLightShadowmapTexturesLocations.end(), DirectionalLightShadowmapTexturesLocationBegin);

    UniformArray<UniformType::Sampler2D, MaxDirectionalLightsPerExecute>(_program, DirectionalLightShadowmapSamplersUniform).Set(directionalLightShadowmapTexturesLocations.data(), MaxDirectionalLightsPerExecute);

    std::array<GLint, MaxSpotLightsPerExecute> spotLightShadowmapTextureLocations;
    std::iota(spotLightShadowmapTextureLocations.begin(), spotLightShadowmapTextureLocations.end(), SpotLightShadowmapTexturesLocationBegin);

    UniformArray<UniformType::Sampler2D, MaxSpotLightsPerExecute>(_program, SpotLightShadowmapSamplersUniform).Set(spotLightShadowmapTextureLocations.data(), MaxSpotLightsPerExecute);
}

LightingFragmentProgram::LightingFragmentProgram(LightingFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._drawDirectionalLight, this->_drawDirectionalLight);
    std::swap(other._drawSpotLight, this->_drawSpotLight);
}

LightingFragmentProgram& LightingFragmentProgram::operator=(LightingFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._drawDirectionalLight, this->_drawDirectionalLight);
    std::swap(other._drawSpotLight, this->_drawSpotLight);
    return *this;
}

LightingFragmentProgram::operator unsigned int() const {
    return _program;
}

void LightingFragmentProgram::prepareShared(const SharedData& data) const {
    glActiveTexture(GL_TEXTURE0 + AlbedoTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.albedoTexture);

    glActiveTexture(GL_TEXTURE0 + PositionTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.positionTexture);

    glActiveTexture(GL_TEXTURE0 + NormalMapTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.normalMapTexture);

    glActiveTexture(GL_TEXTURE0 + MetallicRoughnessTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.metallicRoughnessTexture);

    if (_drawDirectionalLight) {
        const unsigned int directionalLightsCount = glm::min(static_cast<unsigned int>(data.directionalLightsTransforms.size()), MaxDirectionalLightsPerExecute);
        if (directionalLightsCount > 0)
        {    
            for (int i=0; i<directionalLightsCount; ++i) {
                glActiveTexture(GL_TEXTURE0 + DirectionalLightShadowmapTexturesLocationBegin + i);
                glBindTexture(GL_TEXTURE_2D, data.directionalLightsShadowmapTextureIds[i]);
            }

            using namespace Programs::Base;
            UniformValue<UniformType::Vec4>(_program, CameraPositionUniform).Set(data.cameraPosition);
            UniformValue<UniformType::UnsignedInt>(_program, DirectionalLightsCountUniform).Set(directionalLightsCount);
            UniformArray<UniformType::Mat4, MaxDirectionalLightsPerExecute>(_program, DirectionalLightTransformsUniform).Set(data.directionalLightsTransforms.data(), directionalLightsCount);
            UniformArray<UniformType::Vec4, MaxDirectionalLightsPerExecute>(_program, DirectionalLightDirectionsUniform).Set(data.directionalLightsDirections.data(), directionalLightsCount);
            UniformArray<UniformType::Vec4, MaxDirectionalLightsPerExecute>(_program, DirectionalLightColorsUniform).Set(data.directionalLightsColors.data(), directionalLightsCount);
        }
    }

    if (_drawSpotLight) {
        const unsigned int spotLightsCount = glm::min(static_cast<unsigned int>(data.spotLightsTransforms.size()), MaxSpotLightsPerExecute);
        if (spotLightsCount > 0)
        {

            for (int i=0; i<spotLightsCount; ++i) {
                glActiveTexture(GL_TEXTURE0 + SpotLightShadowmapTexturesLocationBegin + i);
                glBindTexture(GL_TEXTURE_2D, data.spotLightsShadowmapTextureIds[i]);
            }

            using namespace Programs::Base;
            UniformValue<UniformType::Vec4>(_program, CameraPositionUniform).Set(data.cameraPosition);
            UniformValue<UniformType::UnsignedInt>(_program, SpotLightsCountUniform).Set(spotLightsCount);
            UniformArray<UniformType::Vec2, MaxSpotLightsPerExecute>(_program, SpotLightInnerOuterConeAnglesUniform).Set(data.spotLightsInnerOuterConeAngle.data(), spotLightsCount);
            UniformArray<UniformType::Mat4, MaxSpotLightsPerExecute>(_program, SpotLightTransformsUniform).Set(data.spotLightsTransforms.data(), spotLightsCount);
            UniformArray<UniformType::Vec4, MaxSpotLightsPerExecute>(_program, SpotLightDirectionsUniform).Set(data.spotLightsDirections.data(), spotLightsCount);
            UniformArray<UniformType::Vec4, MaxSpotLightsPerExecute>(_program, SpotLightPositionsUniform).Set(data.spotLightsPositions.data(), spotLightsCount);
            UniformArray<UniformType::Vec4, MaxSpotLightsPerExecute>(_program, SpotLightColorsUniform).Set(data.spotLightsColors.data(), spotLightsCount);
        }
    }
}

void LightingFragmentProgram::prepareIndividual() const {
    // noop
}



LightingPipeline::LightingPipeline(LightingVertexProgram& vertexProgram, LightingFragmentProgram& fragmentProgram)
: _vertexProgram(vertexProgram)
, _fragmentProgram(fragmentProgram)
{
    glGenProgramPipelines(1, &_pipeline);

    glUseProgramStages(_pipeline, GL_VERTEX_SHADER_BIT, _vertexProgram);
    glUseProgramStages(_pipeline, GL_FRAGMENT_SHADER_BIT, _fragmentProgram);

#ifndef NDEBUG
{
    glValidateProgramPipeline(_pipeline);
    int success;
    glGetProgramPipelineiv(_pipeline, GL_VALIDATE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramPipelineInfoLog(_pipeline, 512, NULL, infoLog);
        std::cerr << "ERROR::PIPELINE::FAIL\n" << infoLog << std::endl;
    }
}
#endif
}

LightingPipeline::~LightingPipeline() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

LightingPipeline::LightingPipeline(LightingPipeline&& other)
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

LightingPipeline& LightingPipeline::operator=(LightingPipeline&& other) {
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

LightingPipeline::operator unsigned int() const {
    return _pipeline;
}

LightingPipeline::ScopedBinding LightingPipeline::Bind() const {
    return { * this };
}

void LightingPipeline::prepareShared(const SharedData& data) const
{
    _vertexProgram.prepareShared(data);
    _fragmentProgram.prepareShared(data);
}

void LightingPipeline::prepareIndividual() const
{
    _vertexProgram.prepareIndividual();
    _fragmentProgram.prepareIndividual();
}



void LightingPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            LightingVertexProgram()
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            LightingFragmentProgram{
                (fragmentProperties & PipelineProperties::LIGHTTYPE_DIRECTIONAL) != 0,
                (fragmentProperties & PipelineProperties::LIGHTTYPE_SPOT) != 0
            }
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, LightingPipeline(vertexProgram, fragmentProgram));
}

const LightingPipeline& LightingPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace LightingPass
} // namespace SceneDrawing
} // namespace Renderer