#include "SceneDrawing/BasePass/BasePassPipelineManager.hpp"
#include <read_file.hpp>

#include <glm/gtc/type_ptr.hpp>

#ifndef BASE_MATERIAL_VERTEX_SOURCE_PATH
#define BASE_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef BASE_MATERIAL_FRAGMENT_SOURCE_PATH
#define BASE_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace BasePass {

BasePassVertexProgram::BasePassVertexProgram(bool skinned) : _isSkinned(skinned) {
    std::vector<const char*> compilerInput { VersionFlag.data() };

    if (_isSkinned) {
        compilerInput.emplace_back(SkinFlag.data());
    }

    const std::string shaderSource = Utils::readFile(BASE_MATERIAL_VERTEX_SOURCE_PATH);
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

BasePassVertexProgram::~BasePassVertexProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

BasePassVertexProgram::BasePassVertexProgram(BasePassVertexProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._isSkinned, this->_isSkinned);
}

BasePassVertexProgram& BasePassVertexProgram::operator=(BasePassVertexProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._isSkinned, this->_isSkinned);
    return *this;
}

BasePassVertexProgram::operator unsigned int() const {
    return _program;
}

void BasePassVertexProgram::PrepareView(const SceneViewData& sceneView) const {
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ViewTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneView.cameraViewTransform));
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ProjectionTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneView.cameraProjectionTransform));
}

void BasePassVertexProgram::PrepareElement(const SceneObjectData& sceneObject) const {
    glProgramUniform1i(_program, glGetUniformLocation(_program, MeshIdUniform.data()), sceneObject.meshId);
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ModelTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneObject.objectModelTransform));
    if (_isSkinned) {
        assert(sceneObject.jointsArray != nullptr);
        glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, JointTransformsUniform.data()), 32, GL_FALSE, glm::value_ptr(*sceneObject.jointsArray->data()));
    }
}



BasePassFragmentProgram::BasePassFragmentProgram(bool hasBaseColorTexture, bool hasNormalMapTexture)
: _hasBaseColorTexture(hasBaseColorTexture)
, _hasNormalMapTexture(hasNormalMapTexture)
{
    std::vector<const char*> compilerInput { VersionFlag.data() };

    if (_hasBaseColorTexture) {
        compilerInput.emplace_back(BaseColorTextureFlag.data());
    }

    if (_hasNormalMapTexture) {
        compilerInput.emplace_back(NormalMapTextureFlag.data());
    }

    const std::string shaderSource = Utils::readFile(BASE_MATERIAL_FRAGMENT_SOURCE_PATH);
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

    if (!success) {
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
    if (_hasBaseColorTexture) {
        glProgramUniform1i(_program, glGetUniformLocation(_program, BaseColorSamplerUniform.data()), GL_TEXTURE0 + BaseColorTextureLocation);
    }

    if (_hasNormalMapTexture) {
        glProgramUniform1i(_program, glGetUniformLocation(_program, NormalMapSamplerUniform.data()), GL_TEXTURE0 + NormalMapTextureLocation);
    }

    // Delete shader as we only need program.
    glDetachShader(_program, shader);
    glDeleteShader(shader);  
}

BasePassFragmentProgram::~BasePassFragmentProgram() {
    if (_program != -1) {
        glDeleteProgram(_program);
    }
}

BasePassFragmentProgram::BasePassFragmentProgram(BasePassFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._hasBaseColorTexture, this->_hasBaseColorTexture);
    std::swap(other._hasNormalMapTexture, this->_hasNormalMapTexture);
}

BasePassFragmentProgram& BasePassFragmentProgram::operator=(BasePassFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._hasBaseColorTexture, this->_hasBaseColorTexture);
    std::swap(other._hasNormalMapTexture, this->_hasNormalMapTexture);
    return *this;
}

BasePassFragmentProgram::operator unsigned int() const {
    return _program;
}

void BasePassFragmentProgram::PrepareView(const SceneViewData& sceneView) const {
    // Do nothing.
}

void BasePassFragmentProgram::PrepareElement(const SceneObjectData& sceneObject) const {
    if (_hasBaseColorTexture) {
        assert(sceneObject.baseColorTexture != nullptr);
        glActiveTexture(GL_TEXTURE0 + BaseColorTextureLocation);
        glBindTexture(GL_TEXTURE_2D, static_cast<unsigned int>(*sceneObject.baseColorTexture));
    }
    
    if (_hasNormalMapTexture) {
        assert(sceneObject.normalMapTexture != nullptr);
        glActiveTexture(GL_TEXTURE0 + NormalMapTextureLocation);
        glBindTexture(GL_TEXTURE_2D, static_cast<unsigned int>(*sceneObject.baseColorTexture));
    }
}


BasePassPipeline::BasePassPipeline(const BasePassVertexProgram& vertexProgram, const BasePassFragmentProgram& fragmentProgram)
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

BasePassPipeline::~BasePassPipeline()
{
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

BasePassPipeline::BasePassPipeline(BasePassPipeline&& other)
: _fragmentProgram(other._fragmentProgram)
, _vertexProgram(other._vertexProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

BasePassPipeline::operator unsigned int() const
{
    return _pipeline;
}

BasePassPipeline::ScopedBinding BasePassPipeline::Bind() const
{
    return { *this };
}

void BasePassPipeline::PrepareView(const SceneViewData& viewData) const 
{
    _vertexProgram.PrepareView(viewData);
    _fragmentProgram.PrepareView(viewData);
}

void BasePassPipeline::PrepareElement(const SceneObjectData& objectData) const
{
    _vertexProgram.PrepareElement(objectData);
    _fragmentProgram.PrepareElement(objectData);
}

void BasePassPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            BasePassVertexProgram(
                (vertexProperties & PipelineProperties::SKIN) != 0
            )
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            BasePassFragmentProgram(
                (fragmentProperties & PipelineProperties::BASE_COLOR_TEXTURE) != 0,
                (fragmentProperties & PipelineProperties::NORMAL_MAP_TEXTURE) != 0
            )
        );
    }

    const auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    const auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _buildPipelines.emplace(properties, BasePassPipeline(vertexProgram, fragmentProgram));
}

const BasePassPipeline& BasePassPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_buildPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _buildPipelines.at(properties);
}

} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer