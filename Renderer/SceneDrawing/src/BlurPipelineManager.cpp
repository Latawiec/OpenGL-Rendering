#include "SceneDrawing/PostProcess/Blur/BlurPipelineManager.hpp"
#include <read_file.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>

#ifndef FORWARD_VERTEX_SOURCE_PATH
#define FORWARD_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef BLUR_MATERIAL_FRAGMENT_SOURCE_PATH
#define BLUR_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace PostProcess {
namespace Blur {

BlurVertexProgram::BlurVertexProgram()
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

BlurVertexProgram::~BlurVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

BlurVertexProgram::BlurVertexProgram(BlurVertexProgram&& other) {
    std::swap(other._program, this->_program);
}

BlurVertexProgram& BlurVertexProgram::operator=(BlurVertexProgram&& other) {
    std::swap(other._program, this->_program);

    return *this;
}

BlurVertexProgram::operator unsigned int() const {
    return _program;
}

void BlurVertexProgram::prepareShared(const SharedData& data) const {
    
}

void BlurVertexProgram::prepareIndividual() const {
    
}


BlurFragmentProgram::BlurFragmentProgram(bool isVertical)
: _isVertical(isVertical)
{
    std::vector<const char*> compilerInput { VersionFlag.data() };

    if (_isVertical) {
        compilerInput.emplace_back(VerticalBlurFlag.data());
    } else {
        compilerInput.emplace_back(HorizontalBlurFlag.data());
    }

    const std::string shaderSource = Utils::readFile(BLUR_MATERIAL_FRAGMENT_SOURCE_PATH);
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

BlurFragmentProgram::~BlurFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

BlurFragmentProgram::BlurFragmentProgram(BlurFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._isVertical, this->_isVertical);
}

BlurFragmentProgram& BlurFragmentProgram::operator=(BlurFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._isVertical, this->_isVertical);
    return *this;
}

BlurFragmentProgram::operator unsigned int() const {
    return _program;
}

void BlurFragmentProgram::prepareShared(const SharedData& data) const {
    glActiveTexture(GL_TEXTURE0 + SourceTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.sourceTexture);
}

void BlurFragmentProgram::prepareIndividual() const {
    // noop
}



BlurPipeline::BlurPipeline(BlurVertexProgram& vertexProgram, BlurFragmentProgram& fragmentProgram)
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

BlurPipeline::~BlurPipeline() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

BlurPipeline::BlurPipeline(BlurPipeline&& other)
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

BlurPipeline& BlurPipeline::operator=(BlurPipeline&& other) {
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

BlurPipeline::operator unsigned int() const {
    return _pipeline;
}

BlurPipeline::ScopedBinding BlurPipeline::Bind() const {
    return { * this };
}

void BlurPipeline::prepareShared(const SharedData& data) const
{
    _vertexProgram.prepareShared(data);
    _fragmentProgram.prepareShared(data);
}

void BlurPipeline::prepareIndividual() const
{
    _vertexProgram.prepareIndividual();
    _fragmentProgram.prepareIndividual();
}


void BlurPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            BlurVertexProgram()
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            BlurFragmentProgram(
                (fragmentProperties & PipelineProperties::VERTICAL_BLUR) != 0
            )
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, BlurPipeline(vertexProgram, fragmentProgram));
}

const BlurPipeline& BlurPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace Blur
} // namespace PostProcess
} // namespace SceneDrawing
} // namespace Renderer