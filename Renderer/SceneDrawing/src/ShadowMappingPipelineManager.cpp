#include "SceneDrawing/ShadowMappingPass/ShadowMappingPassPipelineManager.hpp"
#include "ShaderCompiler/ShaderCompiler.hpp"

#include <glm/gtc/type_ptr.hpp>

#ifndef SHADOWMAPPING_MATERIAL_VERTEX_SOURCE_PATH
#define SHADOWMAPPING_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef SHADOWMAPPING_MATERIAL_FRAGMENT_SOURCE_PATH
#define SHADOWMAPPING_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace ShadowMappingPass {

ShadowMappingVertexProgram::ShadowMappingVertexProgram(const LightType type, bool skinned)
: _type(type)
, _isSkinned(skinned)
{
    Programs::ShaderCompiler::ShaderData data(Programs::ShaderCompiler::ShaderType::Vertex, SHADOWMAPPING_MATERIAL_VERTEX_SOURCE_PATH);

    if (_isSkinned) {
        data.AddFlag(SkinFlag);
    }

    if (_type == LightType::DIRECTIONAL) {
        data.AddFlag(DirectionalLightFlag);
    }

    _program = Programs::ShaderCompiler::Compile(data);
}

ShadowMappingVertexProgram::~ShadowMappingVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

ShadowMappingVertexProgram::ShadowMappingVertexProgram(ShadowMappingVertexProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._isSkinned, this->_isSkinned);
    std::swap(other._type, this->_type);
}

ShadowMappingVertexProgram& ShadowMappingVertexProgram::operator=(ShadowMappingVertexProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._isSkinned, this->_isSkinned);
    std::swap(other._type, this->_type);

    return *this;
}

ShadowMappingVertexProgram::operator unsigned int() const {
    return _program;
}

void ShadowMappingVertexProgram::prepareShared(const SharedData& sceneView) const {
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ViewTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneView.lightViewTransform));
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ProjectionTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneView.lightProjectionTransform));
}

void ShadowMappingVertexProgram::prepareIndividual(const IndividualData& sceneObject) const {
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ModelTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneObject.objectModelTransform));
    if (_isSkinned) {
        assert(sceneObject.jointsArray != nullptr);
        glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, JointTransformsUniform.data()), 32, GL_FALSE, glm::value_ptr(*sceneObject.jointsArray->data()));
    }
}



ShadowMappingFragmentProgram::ShadowMappingFragmentProgram() {
    Programs::ShaderCompiler::ShaderData data(Programs::ShaderCompiler::ShaderType::Fragment, SHADOWMAPPING_MATERIAL_FRAGMENT_SOURCE_PATH);
    _program = Programs::ShaderCompiler::Compile(data);
}

ShadowMappingFragmentProgram::~ShadowMappingFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

ShadowMappingFragmentProgram::ShadowMappingFragmentProgram(ShadowMappingFragmentProgram&& other) {
    std::swap(other._program, this->_program);
}

ShadowMappingFragmentProgram& ShadowMappingFragmentProgram::operator=(ShadowMappingFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    return *this;
}

ShadowMappingFragmentProgram::operator unsigned int() const {
    return _program;
}

void ShadowMappingFragmentProgram::prepareShared(const SharedData& sceneView) const {
    // noop
}

void ShadowMappingFragmentProgram::prepareIndividual(const IndividualData& sceneObject) const {
    // noop
}



ShadowMappingPipeline::ShadowMappingPipeline(ShadowMappingVertexProgram& vertexProgram, ShadowMappingFragmentProgram& fragmentProgram)
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

ShadowMappingPipeline::~ShadowMappingPipeline() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

ShadowMappingPipeline::ShadowMappingPipeline(ShadowMappingPipeline&& other)
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

ShadowMappingPipeline& ShadowMappingPipeline::operator=(ShadowMappingPipeline&& other) {
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

ShadowMappingPipeline::operator unsigned int() const {
    return _pipeline;
}

ShadowMappingPipeline::ScopedBinding ShadowMappingPipeline::Bind() const {
    return { * this };
}

void ShadowMappingPipeline::prepareShared(const SharedData& viewData) const
{
    _vertexProgram.prepareShared(viewData);
    _fragmentProgram.prepareShared(viewData);
}

void ShadowMappingPipeline::prepareIndividual(const IndividualData& objectData) const
{
    _vertexProgram.prepareIndividual(objectData);
    _fragmentProgram.prepareIndividual(objectData);
}



void ShadowMappingPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            ShadowMappingVertexProgram(
                (vertexProperties & PipelineProperties::LIGHTTYPE_DIRECTIONAL) != 0 ? LightType::DIRECTIONAL : LightType::NONE,
                (vertexProperties & PipelineProperties::SKIN) != 0
            )
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            ShadowMappingFragmentProgram()
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, ShadowMappingPipeline(vertexProgram, fragmentProgram));
}

const ShadowMappingPipeline& ShadowMappingPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace ShadowMappingPass
} // namespace SceneDrawing
} // namespace Renderer