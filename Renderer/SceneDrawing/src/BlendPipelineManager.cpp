#include "SceneDrawing/PostProcess/Blend/BlendPipelineManager.hpp"
#include <read_file.hpp>

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

BlendVertexProgram::~BlendVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
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
    std::vector<const char*> compilerInput { VersionFlag.data() };

    if (_additive) {
        compilerInput.emplace_back(AdditiveBlendFlag.data());
    }

    const std::string shaderSource = Utils::readFile(BLEND_MATERIAL_FRAGMENT_SOURCE_PATH);
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
    std::stringstream ss;
    for (int i=0; i<MaxBlendSourcesPerExecute; ++i) {
        ss << SourceSamplersUniform << '[' << i << ']';
        glProgramUniform1i(_program, glGetUniformLocation(_program, ss.str().data()), SourceTextureLocationBegin + i);
        ss.clear();
        ss.str(std::string());
    }

    // Delete shader as we only need program.
    glDetachShader(_program, shader);
    glDeleteShader(shader); 
}

BlendFragmentProgram::~BlendFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
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