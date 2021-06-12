#include "SceneDrawing/BasePass/BasePassPipelineManager.hpp"
#include "ShaderCompiler/ShaderCompiler.hpp"

#include <glm/gtc/type_ptr.hpp>

#ifndef BASEPASS_MATERIAL_VERTEX_SOURCE_PATH
#define BASEPASS_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef BASEPASS_MATERIAL_FRAGMENT_SOURCE_PATH
#define BASEPASS_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Renderer {
namespace SceneDrawing {
namespace BasePass {

BasePassVertexProgram::BasePassVertexProgram(bool skinned, bool normalMap) : _isSkinned(skinned), _hasNormalMapTexture(normalMap) {
    Programs::ShaderCompiler::ShaderData data(Programs::ShaderCompiler::ShaderType::Vertex, BASEPASS_MATERIAL_VERTEX_SOURCE_PATH);

    if (_isSkinned) {
        data.AddFlag(SkinFlag);
    }

    if (_hasNormalMapTexture) {
        data.AddFlag(NormalMapTextureFlag);
    }

    _program = Programs::ShaderCompiler::Compile(data);
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

void BasePassVertexProgram::prepareShared(const SharedData& sceneView) const {
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ViewTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneView.cameraViewTransform));
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ProjectionTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneView.cameraProjectionTransform));
}

void BasePassVertexProgram::prepareIndividual(const IndividualData& sceneObject) const {
    glProgramUniform1i(_program, glGetUniformLocation(_program, MeshIdUniform.data()), sceneObject.meshId);
    glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, ModelTransformUniform.data()), 1, GL_FALSE, glm::value_ptr(sceneObject.objectModelTransform));
    if (_isSkinned) {
        assert(sceneObject.jointsArray != nullptr);
        glProgramUniformMatrix4fv(_program, glGetUniformLocation(_program, JointTransformsUniform.data()), 32, GL_FALSE, glm::value_ptr(*sceneObject.jointsArray->data()));
    }
}



BasePassFragmentProgram::BasePassFragmentProgram(bool hasBaseColorTexture, bool hasNormalMapTexture, bool hasMetallicRoughnessTexture, bool dithering)
: _hasBaseColorTexture(hasBaseColorTexture)
, _hasNormalMapTexture(hasNormalMapTexture)
, _hasMetallicRoughnessTexture(hasMetallicRoughnessTexture)
, _dithering(dithering)
{
    Programs::ShaderCompiler::ShaderData data(Programs::ShaderCompiler::ShaderType::Fragment, BASEPASS_MATERIAL_FRAGMENT_SOURCE_PATH);
    

    if (_hasBaseColorTexture) {
        data.AddFlag(BaseColorTextureFlag);
    }

    if (_hasNormalMapTexture) {
        data.AddFlag(NormalMapTextureFlag);
    }

    if (_hasMetallicRoughnessTexture) {
        data.AddFlag(MetallicRoughnessTextureFlag);
    }

    if (_dithering) {
        data.AddFlag(DitheringFlag);
    }

    _program = Programs::ShaderCompiler::Compile(data);

        // Setup textures
    if (_hasBaseColorTexture) {
        glProgramUniform1i(_program, glGetUniformLocation(_program, BaseColorSamplerUniform.data()), BaseColorTextureLocation);
    }

    if (_hasNormalMapTexture) {
        glProgramUniform1i(_program, glGetUniformLocation(_program, NormalMapSamplerUniform.data()), NormalMapTextureLocation);
    }

    if (_hasMetallicRoughnessTexture) {
        glProgramUniform1i(_program, glGetUniformLocation(_program, MetallicRougnessSamplerUniform.data()), MetallicRoughnessTextureLocation);
    }

    if (_dithering) {
        glProgramUniform1i(_program, glGetUniformLocation(_program, DitheringSamplerUniform.data()), DitherTextureLocation);
    }

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
    std::swap(other._hasMetallicRoughnessTexture, this->_hasMetallicRoughnessTexture);
    std::swap(other._dithering, this->_dithering);
}

BasePassFragmentProgram& BasePassFragmentProgram::operator=(BasePassFragmentProgram&& other) {
    std::swap(other._program, this->_program);
    std::swap(other._hasBaseColorTexture, this->_hasBaseColorTexture);
    std::swap(other._hasNormalMapTexture, this->_hasNormalMapTexture);
    std::swap(other._hasMetallicRoughnessTexture, this->_hasMetallicRoughnessTexture);
    std::swap(other._dithering, this->_dithering);
    return *this;
}

BasePassFragmentProgram::operator unsigned int() const {
    return _program;
}

void BasePassFragmentProgram::prepareShared(const SharedData& sceneView) const {
    // Do nothing.
}

void BasePassFragmentProgram::prepareIndividual(const IndividualData& sceneObject) const {
    if (_hasBaseColorTexture) {
        assert(sceneObject.baseColorTexture != nullptr);
        glActiveTexture(GL_TEXTURE0 + BaseColorTextureLocation);
        glBindTexture(GL_TEXTURE_2D, static_cast<unsigned int>(*sceneObject.baseColorTexture));
    }
    
    if (_hasNormalMapTexture) {
        assert(sceneObject.normalMapTexture != nullptr);
        glActiveTexture(GL_TEXTURE0 + NormalMapTextureLocation);
        glBindTexture(GL_TEXTURE_2D, static_cast<unsigned int>(*sceneObject.normalMapTexture));
    }

    if (_hasMetallicRoughnessTexture) {
        assert(sceneObject.metallicRoughnessTexture != nullptr);
        glActiveTexture(GL_TEXTURE0 + MetallicRoughnessTextureLocation);
        glBindTexture(GL_TEXTURE_2D, static_cast<unsigned int>(*sceneObject.metallicRoughnessTexture));
    }

    if (_dithering) {
        assert(sceneObject.ditherTexture != nullptr);
        glActiveTexture(GL_TEXTURE0 + DitherTextureLocation);
        glBindTexture(GL_TEXTURE_2D, static_cast<unsigned int>(*sceneObject.ditherTexture));
    }
}


BasePassPipeline::BasePassPipeline(BasePassVertexProgram& vertexProgram, BasePassFragmentProgram& fragmentProgram)
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
: _vertexProgram(other._vertexProgram)
, _fragmentProgram(other._fragmentProgram)
{
    std::swap(this->_pipeline, other._pipeline);
}

BasePassPipeline& BasePassPipeline::operator=(BasePassPipeline&& other)
{
    std::swap(this->_vertexProgram, other._vertexProgram);
    std::swap(this->_fragmentProgram, other._fragmentProgram);
    std::swap(this->_pipeline, other._pipeline);

    return *this;
}

BasePassPipeline::operator unsigned int() const
{
    return _pipeline;
}

BasePassPipeline::ScopedBinding BasePassPipeline::Bind() const
{
    return { *this };
}

void BasePassPipeline::prepareShared(const SharedData& viewData) const 
{
    _vertexProgram.prepareShared(viewData);
    _fragmentProgram.prepareShared(viewData);
}

void BasePassPipeline::prepareIndividual(const IndividualData& objectData) const
{
    _vertexProgram.prepareIndividual(objectData);
    _fragmentProgram.prepareIndividual(objectData);
}

void BasePassPipelineManager::buildVariant(const PropertiesSet& properties)
{
    const auto vertexProperties = properties & PropertiesAffectingVertexProgram;
    const auto fragmentProperties = properties & PropertiesAffectingFragmentProgram;

    if (!_cachedVertexPrograms.contains(vertexProperties)) {
        _cachedVertexPrograms.emplace(
            vertexProperties,
            BasePassVertexProgram(
                (vertexProperties & PipelineProperties::SKIN) != 0,
                (vertexProperties & PipelineProperties::NORMAL_MAP_TEXTURE) != 0
            )
        );
    }

    if (!_cachedFragmentPrograms.contains(fragmentProperties)) {
        _cachedFragmentPrograms.emplace(
            fragmentProperties,
            BasePassFragmentProgram(
                (fragmentProperties & PipelineProperties::BASE_COLOR_TEXTURE) != 0,
                (fragmentProperties & PipelineProperties::NORMAL_MAP_TEXTURE) != 0,
                (fragmentProperties & PipelineProperties::METALLIC_ROUGHNESS_TEXTURE) != 0,
                (fragmentProperties & PipelineProperties::DITHERING) != 0
            )
        );
    }

    auto& vertexProgram = _cachedVertexPrograms.at(vertexProperties);
    auto& fragmentProgram = _cachedFragmentPrograms.at(fragmentProperties);

    _builtPipelines.emplace(properties, BasePassPipeline(vertexProgram, fragmentProgram));
}

const BasePassPipeline& BasePassPipelineManager::GetPipeline(const PropertiesSet& properties)
{
    if (!_builtPipelines.contains(properties)) {
        buildVariant(properties);
    } 
    return _builtPipelines.at(properties);
}

} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer