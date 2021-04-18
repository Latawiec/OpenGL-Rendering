#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <id_generator.hpp>
#include <Scene/Scene.hpp>
#include <Scene/NodeLink.hpp>
#include <EdgeProgram.hpp>
#include <TextureProgram.hpp>
#include "GraphicBuffer.hpp"
#include "TransformProcessor.hpp"
#include "BasePass/BasePassPipelineManager.hpp"


namespace Renderer {
namespace SceneDrawing {

struct SceneDrawingManager {

SceneDrawingManager(const Renderer::Scene::Scene& scene, const int windowWidth, const int windowHeight);

void Draw();

private:
    const Renderer::Scene::Scene& _scene;

    GraphicBuffer _deferredBuffers;
    
    TransformProcessor _transformProcessor;
    BasePass::BasePassPipelineManager _basePassPipelineManager;

    void prepareSkin(const Renderer::Scene::Base::Skin::IdType& skinId);
    std::unordered_map<Renderer::Scene::Base::Skin::IdType, std::array<glm::mat4, 32>> _jointTransforms;

    int _width, _height;

    // Debugging, testing ... seeing if works.
    Renderer::Programs::TextureProgram _textureDrawProgram;
    Renderer::Programs::EdgeProgram _edgeProgram;

    void BasePass();
};

} // namespace SceneDrawing
} // namespace Renderer