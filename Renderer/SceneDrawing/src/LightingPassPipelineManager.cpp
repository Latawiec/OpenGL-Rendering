#include "SceneDrawing/LightingPass/LightingPassPipelineManager.hpp"
#include <read_file.hpp>

#include <glm/gtc/type_ptr.hpp>
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
    std::vector<const char*> compilerInput { VersionFlag.data() };

    const std::string shaderSource = Utils::readFile(LIGHTING_MATERIAL_VERTEX_SOURCE_PATH);
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

LightingVertexProgram::~LightingVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
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



LightingFragmentProgram::LightingFragmentProgram(const LightType type)
: _type(type)
{
    std::vector<const char*> compilerInput { VersionFlag.data() };

    if (_type == LightType::DIRECTIONAL) {
        compilerInput.emplace_back(DirectionalLightFlag.data());
    }

    const std::string shaderSource = Utils::readFile(LIGHTING_MATERIAL_FRAGMENT_SOURCE_PATH);
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
    glProgramUniform1i(_program, glGetUniformLocation(_program, PositionSamplerUniform.data()), PositionTextureLocation);
    glProgramUniform1i(_program, glGetUniformLocation(_program, NormalMapSamplerUniform.data()), NormalMapTextureLocation);
    std::stringstream ss;
    for (int i=0; i<MaxDirectionalLightsPerExecute; ++i) {
        ss << DirectionalLightShadowmapSamplersUniform << '[' << i << ']';
        glProgramUniform1i(_program, glGetUniformLocation(_program, ss.str().data()), DirectionalLightShadowmapTexturesLocationBegin + i);
        ss.clear();
    }

    // Delete shader as we only need program.
    glDetachShader(_program, shader);
    glDeleteShader(shader); 
}

LightingFragmentProgram::~LightingFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

LightingFragmentProgram::LightingFragmentProgram(LightingFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._type, this->_type);
}

LightingFragmentProgram& LightingFragmentProgram::operator=(LightingFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._type, this->_type);
    return *this;
}

LightingFragmentProgram::operator unsigned int() const {
    return _program;
}

void LightingFragmentProgram::prepareShared(const SharedData& data) const {
    glActiveTexture(GL_TEXTURE0 + PositionTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.positionTexture);

    glActiveTexture(GL_TEXTURE0 + NormalMapTextureLocation);
    glBindTexture(GL_TEXTURE_2D, data.normalMapTexture);

    const unsigned int directionalLightsCount = glm::min(static_cast<unsigned int>(data.directionalLightsTransforms.size()), MaxDirectionalLightsPerExecute);
    glProgramUniform1ui(_program, glGetUniformLocation(_program, DirectionalLightsCountUniform.data()), directionalLightsCount);

    for (int i=0; i<directionalLightsCount; ++i) {
        glActiveTexture(GL_TEXTURE0 + DirectionalLightShadowmapTexturesLocationBegin + i);
        glBindTexture(GL_TEXTURE_2D, data.directionalLightsShadowmapTextureIds[i]);

        std::stringstream ss;
        ss << DirectionalLightTransformsUniform << '[' << i << ']';
        glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ss.str().data()), 1, GL_FALSE, glm::value_ptr(data.directionalLightsTransforms[i]));
        ss.clear();
        ss.str(std::string());
        ss << DirectionalLightDirectionsUniform << '[' << i << ']';
        glProgramUniform4fv(_program, glGetUniformLocation(_program, ss.str().data()), 1, glm::value_ptr(data.directionalLightsDirections[i]));
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
                (fragmentProperties & PipelineProperties::LIGHTTYPE_DIRECTIONAL) != 0 ? LightType::DIRECTIONAL : LightType::NONE,
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