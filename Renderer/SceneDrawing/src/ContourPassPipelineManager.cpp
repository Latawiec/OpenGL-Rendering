#include "SceneDrawing/ContourPass/ContourPassPipelineManager.hpp"
#include <read_file.hpp>

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
    std::vector<const char*> compilerInput { VersionFlag.data() };

    const std::string shaderSource = Utils::readFile(CONTOUR_MATERIAL_VERTEX_SOURCE_PATH);
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
    std::vector<const char*> compilerInput { VersionFlag.data() };

    const std::string shaderSource = Utils::readFile(CONTOUR_MATERIAL_FRAGMENT_SOURCE_PATH);
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
    glProgramUniform1i(_program, glGetUniformLocation(_program, SilhouetteSamplerUniform.data()), SilhouetteTextureLocation);

    // Delete shader as we only need program.
    glDetachShader(_program, shader);
    glDeleteShader(shader); 
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