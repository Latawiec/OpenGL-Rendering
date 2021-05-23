#include "SceneDrawing/PostProcess/BrightnessFilter/BrightnessFilterPipelineManager.hpp"
#include <read_file.hpp>

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
    std::vector<const char*> compilerInput { VersionFlag.data() };

    const std::string shaderSource = Utils::readFile(FORWARD_VERTEX_SOURCE_PATH);
    compilerInput.emplace_back(shaderSource.data());

    _program = glCreateProgram();
    glProgramParameteri(_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, compilerInput.size(), compilerInput.data(), NULL);
    glCompileShader(shader);

#ifndef NDEBUG
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
#endif

    glAttachShader(_program, shader);
    glLinkProgram(_program);

#ifndef NDEBUG
{
    int success;
    char infoLog[512];
    glGetProgramiv(_program, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        glGetProgramInfoLog(_program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}
#endif

    // Delete shader as we only need program.
    glDetachShader(_program, shader);
    glDeleteShader(shader); 
}

BrightnessFilterVertexProgram::~BrightnessFilterVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
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
    std::vector<const char*> compilerInput { VersionFlag.data() };

    if (_nullifyBelowTreshold) {
        compilerInput.emplace_back(NullifyBelowFlag.data());
    }

    const std::string shaderSource = Utils::readFile(BRIGHTNESS_FILTER_MATERIAL_FRAGMENT_SOURCE_PATH);
    compilerInput.emplace_back(shaderSource.data());

    _program = glCreateProgram();
    glProgramParameteri(_program, GL_PROGRAM_SEPARABLE, GL_TRUE);

    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, compilerInput.size(), compilerInput.data(), NULL);
    glCompileShader(shader);

#ifndef NDEBUG
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
#endif

    glAttachShader(_program, shader);
    glLinkProgram(_program);

#ifndef NDEBUG
{
    int success;
    char infoLog[512];
    glGetProgramiv(_program, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        glGetProgramInfoLog(_program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}
#endif

    // Setup textures
    glProgramUniform1i(_program, glGetUniformLocation(_program, SourceSamplerUniform.data()), SourceTextureLocation);

    // Delete shader as we only need program.
    glDetachShader(_program, shader);
    glDeleteShader(shader); 
}

BrightnessFilterFragmentProgram::~BrightnessFilterFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
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