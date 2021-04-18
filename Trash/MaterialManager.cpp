#include "SceneDrawing/MaterialManager.hpp"


#ifndef BASE_MATERIAL_VERTEX_SOURCE_PATH
#define BASE_MATERIAL_VERTEX_SOURCE_PATH "Invalid vertex shader source path."
#endif 

#ifndef BASE_MATERIAL_FRAGMENT_SOURCE_PATH
#define BASE_MATERIAL_FRAGMENT_SOURCE_PATH "Invalid fragment shader source path."
#endif 


namespace Render {
namespace SceneDrawing {

// Pipeline
Pipeline::Pipeline(const Program<ProgramType::Vertex>& vertexProgram, const Program<ProgramType::Fragment>& fragmentProgram)
{
    glGenProgramPipelines(1, &_pipeline);

    glUseProgramStages(_pipeline, GL_VERTEX_SHADER_BIT, vertexProgram);
    glUseProgramStages(_pipeline, GL_FRAGMENT_SHADER_BIT, fragmentProgram);
}

Pipeline::~Pipeline()
{
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

Pipeline::Pipeline(Pipeline&& other)
{
    const auto hold = _pipeline;
    this->_pipeline = other._pipeline;
    other._pipeline = hold;
}

Pipeline& Pipeline::operator=(Pipeline&& other)
{
    const auto hold = _pipeline;
    this->_pipeline = other._pipeline;
    other._pipeline = hold;

    return *this;
}

Pipeline::operator unsigned int() const
{
    return _pipeline;
}

Pipeline::ScopedBinding Pipeline::Bind() const
{
    return { *this };
}

// MaterialManager

void MaterialManager::Draw(const Common::Scene::SceneObject& sceneObject, const Common::Scene::SceneView& view, const Common::Scene& scene) const 
{
    if (sceneElement.meshId == Mesh::INVALID_ID) {
        // Can't draw without geometry.
        return;
    }

    if (sceneElement.materialId == Material::INVALID_ID) {
        // Can't draw without material for now. Maybe I'll replace it with a dummy material later on.
        return;
    }
}

Pipeline& MaterialManager::PreparePipeline(const Common::Scene::SceneObject& sceneObject, const Common::Scene::SceneView& view, const Common::Scene& scene) const 
{
    PropertiesSet set = 0;

    if (sceneObject.skinId != Skin::INVALID_ID) {
        set |= Properties::SKIN;
    }

    const auto& material = scene.GetMaterial(sceneObject.)

    if (sceneObject.)

}

const Pipeline& MaterialManager::GetPipeline(const Material::PropertiesSet properties)
{
    if (_pipelinesCache.contains(properties)) {
        return _pipelinesCache.at(properties);
    } else {
        return compileVariant(properties);
    }
}

MaterialManager::~MaterialManager()
{
    _pipelinesCache.clear();
    _vertexStagePrograms.clear();
    _fragmentStagePrograms.clear();
}

Pipeline& MaterialManager::compileVariant(const Material::PropertiesSet properties)
{
    const auto isCorrectVertexVariantCompiled = _vertexStagePrograms.contains(properties & VertexShaderVariantsMask);
    if (!isCorrectVertexVariantCompiled) {
        const auto variantFlags = properties & VertexShaderVariantsMask;
        _vertexStagePrograms.emplace(variantFlags, Program<ProgramType::Vertex>(BASE_MATERIAL_VERTEX_SOURCE_PATH, variantFlags));
    }
    const auto& vertexProgram = _vertexStagePrograms.at(properties & VertexShaderVariantsMask);

    const auto isCorrectFragmentVariantCompiled = _fragmentStagePrograms.contains(properties & FragmentShaderVariantsMask);
    if (!isCorrectFragmentVariantCompiled) {
        const auto variantFlags = properties & FragmentShaderVariantsMask;
        _fragmentStagePrograms.emplace(variantFlags, Program<ProgramType::Fragment>(BASE_MATERIAL_FRAGMENT_SOURCE_PATH, variantFlags));
    }
    const auto& fragmentProgram = _fragmentStagePrograms.at(properties & FragmentShaderVariantsMask);

    return _pipelinesCache.emplace(properties, Pipeline(vertexProgram, fragmentProgram)).first->second;
}

} // namespace Common
} // namespace Render
