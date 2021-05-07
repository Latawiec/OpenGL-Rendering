#include "SceneDrawing/CombinePass/CombinePassPipelineManager.hpp"
#include <read_file.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#ifndef COMBINE_MATERIAL_VERTEX_SOURCE_PATH
#define COMBINE_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef COMBINE_MATERIAL_FRAGMENT_SOURCE_PATH
#define COMBINE_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace CombinePass {

CombineVertexProgram::CombineVertexProgram()
{
    std::vector<const char*> compilerInput { VersionFlag.data() };

    const std::string shaderSource = Utils::readFile(COMBINE_MATERIAL_VERTEX_SOURCE_PATH);
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

CombineVertexProgram::~CombineVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

CombineVertexProgram::CombineVertexProgram(CombineVertexProgram&& other) {
    std::swap(other._program, this->_program);
}

CombineVertexProgram& CombineVertexProgram::operator=(CombineVertexProgram&& other) {
    std::swap(other._program, this->_program);

    return *this;
}

CombineVertexProgram::operator unsigned int() const {
    return _program;
}

void CombineVertexProgram::prepareShared(const SharedData& data) const {
    
}

void CombineVertexProgram::prepareIndividual() const {
    
}


CombineFragmentProgram::CombineFragmentProgram()
{
    std::vector<const char*> compilerInput { VersionFlag.data() };

    const std::string shaderSource = Utils::readFile(COMBINE_MATERIAL_FRAGMENT_SOURCE_PATH);
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
    glProgramUniform1i(_program, glGetUniformLocation(_program, AlbedoSamplerUniform.data()), AlbedoTextureLocation);
    glProgramUniform1i(_program, glGetUniformLocation(_program, DiffuseSamplerUniform.data()), DiffuseTextureLocation);
    glProgramUniform1i(_program, glGetUniformLocation(_program, SpecularSamplerUniform.data()), SpecularTextureLocation);
    glProgramUniform1i(_program, glGetUniformLocation(_program, DitheringSamplerUniform.data()), DitherTextureLocation);

    // Delete shader as we only need program.
    glDetachShader(_program, shader);
    glDeleteShader(shader); 
}

CombineFragmentProgram::~CombineFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

CombineFragmentProgram::CombineFragmentProgram(CombineFragmentProgram&& other) {
    std::swap(other._program, this->_program);
}

CombineFragmentProgram& CombineFragmentProgram::operator=(CombineFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    return *this;
}

CombineFragmentProgram::operator unsigned int() const {
    return _program;
}

void CombineFragmentProgram::prepareShared(const SharedData& data) const {
    glActiveTexture(GL_TEXTURE0 + AlbedoTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.albedoTexture);

    glActiveTexture(GL_TEXTURE0 + DiffuseTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.diffuseTexture);

    glActiveTexture(GL_TEXTURE0 + SpecularTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.specularTexture);

    glActiveTexture(GL_TEXTURE0 + DitherTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.ditherTexture);
}

void CombineFragmentProgram::prepareIndividual() const {
    // noop
}



CombinePipeline::CombinePipeline(CombineVertexProgram& vertexProgram, CombineFragmentProgram& fragmentProgram)
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

CombinePipeline::~CombinePipeline() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

CombinePipeline::CombinePipeline(CombinePipeline&& other)
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

CombinePipeline& CombinePipeline::operator=(CombinePipeline&& other) {
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

CombinePipeline::operator unsigned int() const {
    return _pipeline;
}

CombinePipeline::ScopedBinding CombinePipeline::Bind() const {
    return { * this };
}

void CombinePipeline::prepareShared(const SharedData& data) const
{
    _vertexProgram.prepareShared(data);
    _fragmentProgram.prepareShared(data);
}

void CombinePipeline::prepareIndividual() const
{
    _vertexProgram.prepareIndividual();
    _fragmentProgram.prepareIndividual();
}


void CombinePipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            CombineVertexProgram()
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            CombineFragmentProgram()
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, CombinePipeline(vertexProgram, fragmentProgram));
}

const CombinePipeline& CombinePipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace LightingPass
} // namespace SceneDrawing
} // namespace Renderer