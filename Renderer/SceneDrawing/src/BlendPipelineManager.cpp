#include "SceneDrawing/PostProcess/Blend/BlendPipelineManager.hpp"
#include "Base/ShaderCompiler.hpp"
#include "Base/UniformValue.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>

#ifndef FORWARD_VERTEX_SOURCE_PATH
#define FORWARD_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef BLEND_MATERIAL_FRAGMENT_SOURCE_PATH
#define BLEND_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace PostProcess {
namespace Blend {

BlendVertexProgram::BlendVertexProgram()
{
    Programs::Base::ShaderData<Programs::Base::ShaderType::Vertex> data(FORWARD_VERTEX_SOURCE_PATH);
    _program = Programs::Base::Compile(data);
}

BlendVertexProgram::BlendVertexProgram(BlendVertexProgram&& other) {
    std::swap(other._program, this->_program);
}

BlendVertexProgram& BlendVertexProgram::operator=(BlendVertexProgram&& other) {
    std::swap(other._program, this->_program);

    return *this;
}

BlendVertexProgram::operator unsigned int() const {
    return _program;
}

void BlendVertexProgram::prepareShared(const SharedData& data) const {
    
}

void BlendVertexProgram::prepareIndividual() const {
    
}


BlendFragmentProgram::BlendFragmentProgram(bool additive)
: _additive(additive)
{
    Programs::Base::ShaderData<Programs::Base::ShaderType::Fragment> data(BLEND_MATERIAL_FRAGMENT_SOURCE_PATH);

    if (_additive) {
        data.AddFlag(AdditiveBlendFlag);
    }

    _program = Programs::Base::Compile(data);

        // Setup textures
    std::stringstream ss;
    for (int i=0; i<MaxBlendSourcesPerExecute; ++i) {
        ss << SourceSamplersUniform << '[' << i << ']';
        glProgramUniform1i(_program, glGetUniformLocation(_program, ss.str().data()), SourceTextureLocationBegin + i);
        ss.clear();
        ss.str(std::string());
    }
    
}

BlendFragmentProgram::BlendFragmentProgram(BlendFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._additive, this->_additive);
}

BlendFragmentProgram& BlendFragmentProgram::operator=(BlendFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._additive, this->_additive);
    return *this;
}

BlendFragmentProgram::operator unsigned int() const {
    return _program;
}

void BlendFragmentProgram::prepareShared(const SharedData& data) const {

    const unsigned int blendSourcesCount = glm::min(data.sourcesCount, MaxBlendSourcesPerExecute);
    glProgramUniform1ui(_program, glGetUniformLocation(_program, SourcesCountUniform.data()), blendSourcesCount);

    for (int i=0; i<blendSourcesCount; ++i) {
        glActiveTexture(GL_TEXTURE0 + SourceTextureLocationBegin + i);
        glBindTexture(GL_TEXTURE_2D, data.sourceTextures[i]);
    }
}

void BlendFragmentProgram::prepareIndividual() const {
    // noop
}



BlendPipeline::BlendPipeline(BlendVertexProgram& vertexProgram, BlendFragmentProgram& fragmentProgram)
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

BlendPipeline::~BlendPipeline() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

BlendPipeline::BlendPipeline(BlendPipeline&& other)
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

BlendPipeline& BlendPipeline::operator=(BlendPipeline&& other) {
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

BlendPipeline::operator unsigned int() const {
    return _pipeline;
}

BlendPipeline::ScopedBinding BlendPipeline::Bind() const {
    return { * this };
}

void BlendPipeline::prepareShared(const SharedData& data) const
{
    _vertexProgram.prepareShared(data);
    _fragmentProgram.prepareShared(data);
}

void BlendPipeline::prepareIndividual() const
{
    _vertexProgram.prepareIndividual();
    _fragmentProgram.prepareIndividual();
}


void BlendPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            BlendVertexProgram()
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            BlendFragmentProgram(
                (fragmentProperties & PipelineProperties::ADDITIVE) != 0
            )
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, BlendPipeline(vertexProgram, fragmentProgram));
}

const BlendPipeline& BlendPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace Blend
} // namespace PostProcess
} // namespace SceneDrawing
} // namespace Renderer