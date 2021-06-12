#include "SceneDrawing/ContourPass/ContourPassPipelineManager.hpp"
#include "ShaderCompiler/ShaderCompiler.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#ifndef CONTOUR_MATERIAL_VERTEX_SOURCE_PATH
#define CONTOUR_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef CONTOUR_MATERIAL_FRAGMENT_SOURCE_PATH
#define CONTOUR_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace ContourPass {

ContourVertexProgram::ContourVertexProgram()
{
    Programs::ShaderCompiler::ShaderData data(Programs::ShaderCompiler::ShaderType::Vertex, CONTOUR_MATERIAL_VERTEX_SOURCE_PATH);
    _program = Programs::ShaderCompiler::Compile(data);
}

ContourVertexProgram::~ContourVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

ContourVertexProgram::ContourVertexProgram(ContourVertexProgram&& other) {
    std::swap(other._program, this->_program);
}

ContourVertexProgram& ContourVertexProgram::operator=(ContourVertexProgram&& other) {
    std::swap(other._program, this->_program);

    return *this;
}

ContourVertexProgram::operator unsigned int() const {
    return _program;
}

void ContourVertexProgram::prepareShared(const SharedData& data) const {
    
}

void ContourVertexProgram::prepareIndividual() const {
    
}


ContourFragmentProgram::ContourFragmentProgram()
{
    Programs::ShaderCompiler::ShaderData data(Programs::ShaderCompiler::ShaderType::Fragment, CONTOUR_MATERIAL_FRAGMENT_SOURCE_PATH);
    _program = Programs::ShaderCompiler::Compile(data);
 
    // Setup textures
    glProgramUniform1i(_program, glGetUniformLocation(_program, SilhouetteSamplerUniform.data()), SilhouetteTextureLocation);
    glProgramUniform1i(_program, glGetUniformLocation(_program, NormalMapSamplerUniform.data()), NormalMapTextureLocation);
    glProgramUniform1i(_program, glGetUniformLocation(_program, DepthSamplerUniform.data()), DepthTextureLocation);
}

ContourFragmentProgram::~ContourFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

ContourFragmentProgram::ContourFragmentProgram(ContourFragmentProgram&& other) {
    std::swap(other._program, this->_program);
}

ContourFragmentProgram& ContourFragmentProgram::operator=(ContourFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    return *this;
}

ContourFragmentProgram::operator unsigned int() const {
    return _program;
}

void ContourFragmentProgram::prepareShared(const SharedData& data) const {
    glActiveTexture(GL_TEXTURE0 + SilhouetteTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.silhouetteTexture);

    glActiveTexture(GL_TEXTURE0 + NormalMapTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.normalMapTexture);

    glActiveTexture(GL_TEXTURE0 + DepthTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.depthTexture);

    glProgramUniform2fv(_program, glGetUniformLocation(_program, CameraNearFarUniform.data()), 1, glm::value_ptr(data.cameraNearFar));
}

void ContourFragmentProgram::prepareIndividual() const {
    // noop
}



ContourPipeline::ContourPipeline(ContourVertexProgram& vertexProgram, ContourFragmentProgram& fragmentProgram)
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

ContourPipeline::~ContourPipeline() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

ContourPipeline::ContourPipeline(ContourPipeline&& other)
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

ContourPipeline& ContourPipeline::operator=(ContourPipeline&& other) {
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

ContourPipeline::operator unsigned int() const {
    return _pipeline;
}

ContourPipeline::ScopedBinding ContourPipeline::Bind() const {
    return { * this };
}

void ContourPipeline::prepareShared(const SharedData& data) const
{
    _vertexProgram.prepareShared(data);
    _fragmentProgram.prepareShared(data);
}

void ContourPipeline::prepareIndividual() const
{
    _vertexProgram.prepareIndividual();
    _fragmentProgram.prepareIndividual();
}


void ContourPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            ContourVertexProgram()
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            ContourFragmentProgram()
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, ContourPipeline(vertexProgram, fragmentProgram));
}

const ContourPipeline& ContourPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace LightingPass
} // namespace SceneDrawing
} // namespace Renderer