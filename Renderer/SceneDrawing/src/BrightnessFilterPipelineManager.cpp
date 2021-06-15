#include "SceneDrawing/PostProcess/BrightnessFilter/BrightnessFilterPipelineManager.hpp"
#include "Base/ShaderCompiler.hpp"
#include "Base/UniformValue.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>

#ifndef FORWARD_VERTEX_SOURCE_PATH
#define FORWARD_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef BRIGHTNESS_FILTER_MATERIAL_FRAGMENT_SOURCE_PATH
#define BRIGHTNESS_FILTER_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace PostProcess {
namespace BrightnessFilter {

BrightnessFilterVertexProgram::BrightnessFilterVertexProgram()
{
    Programs::Base::ShaderData<Programs::Base::ShaderType::Vertex> data(FORWARD_VERTEX_SOURCE_PATH);
    _program = Programs::Base::Compile(data);
}

BrightnessFilterVertexProgram::BrightnessFilterVertexProgram(BrightnessFilterVertexProgram&& other) {
    std::swap(other._program, this->_program);
}

BrightnessFilterVertexProgram& BrightnessFilterVertexProgram::operator=(BrightnessFilterVertexProgram&& other) {
    std::swap(other._program, this->_program);

    return *this;
}

BrightnessFilterVertexProgram::operator unsigned int() const {
    return _program;
}

void BrightnessFilterVertexProgram::prepareShared(const SharedData& data) const {
    
}

void BrightnessFilterVertexProgram::prepareIndividual() const {
    
}


BrightnessFilterFragmentProgram::BrightnessFilterFragmentProgram(bool nullifyBelowTreshold)
: _nullifyBelowTreshold(nullifyBelowTreshold)
{
    Programs::Base::ShaderData<Programs::Base::ShaderType::Fragment> data(BRIGHTNESS_FILTER_MATERIAL_FRAGMENT_SOURCE_PATH);

    if (_nullifyBelowTreshold) { data.AddFlag(NullifyBelowFlag); }

    _program = Programs::Base::Compile(data);

    // Setup textures
    Programs::Base::UniformValue<Programs::Base::UniformType::Sampler2D>(_program, SourceSamplerUniform).Set(SourceTextureLocation);
}

BrightnessFilterFragmentProgram::BrightnessFilterFragmentProgram(BrightnessFilterFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._nullifyBelowTreshold, this->_nullifyBelowTreshold);
}

BrightnessFilterFragmentProgram& BrightnessFilterFragmentProgram::operator=(BrightnessFilterFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._nullifyBelowTreshold, this->_nullifyBelowTreshold);
    return *this;
}

BrightnessFilterFragmentProgram::operator unsigned int() const {
    return _program;
}

void BrightnessFilterFragmentProgram::prepareShared(const SharedData& data) const {
    glActiveTexture(GL_TEXTURE0 + SourceTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.sourceTexture);

    glProgramUniform1f(_program, glGetUniformLocation(_program, BrightnessTresholdUniform.data()), data.filterTreshold);
}

void BrightnessFilterFragmentProgram::prepareIndividual() const {
    // noop
}



BrightnessFilterPipeline::BrightnessFilterPipeline(BrightnessFilterVertexProgram& vertexProgram, BrightnessFilterFragmentProgram& fragmentProgram)
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

BrightnessFilterPipeline::~BrightnessFilterPipeline() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

BrightnessFilterPipeline::BrightnessFilterPipeline(BrightnessFilterPipeline&& other)
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

BrightnessFilterPipeline& BrightnessFilterPipeline::operator=(BrightnessFilterPipeline&& other) {
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

BrightnessFilterPipeline::operator unsigned int() const {
    return _pipeline;
}

BrightnessFilterPipeline::ScopedBinding BrightnessFilterPipeline::Bind() const {
    return { * this };
}

void BrightnessFilterPipeline::prepareShared(const SharedData& data) const
{
    _vertexProgram.prepareShared(data);
    _fragmentProgram.prepareShared(data);
}

void BrightnessFilterPipeline::prepareIndividual() const
{
    _vertexProgram.prepareIndividual();
    _fragmentProgram.prepareIndividual();
}


void BrightnessFilterPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            BrightnessFilterVertexProgram()
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            BrightnessFilterFragmentProgram(
                (fragmentProperties & PipelineProperties::NULLIFY_BELOW) != 0
            )
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, BrightnessFilterPipeline(vertexProgram, fragmentProgram));
}

const BrightnessFilterPipeline& BrightnessFilterPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace BrightnessFilter
} // namespace PostProcess
} // namespace SceneDrawing
} // namespace Renderer