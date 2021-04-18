#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <Scene/IdGenerator.hpp>
#include <Scene/Scene.hpp>
#include <Scene/NodeLink.hpp>
#include <Scene/Transformed.hpp>
#include <MappingProgram/Static.hpp>
#include <MappingProgram/Animated.hpp>
#include <EdgeDetector/Program.hpp>
#include "GraphicBuffer.hpp"
#include "TransformProcessor.hpp"
#include "BasePass/BasePassPipelineManager.hpp"

#include "TextureProgram.hpp"

namespace SceneDrawing {

struct SceneDrawingManager {

SceneDrawingManager(const Render::Common::Scene& scene, const int windowWidth, const int windowHeight);

void Draw();

private:
    const Render::Common::Scene& _scene;

    GraphicBuffer _deferredBuffers;
    
    TransformProcessor _transformProcessor;
    BasePass::BasePassPipelineManager _basePassPipelineManager;

    void prepareSkin(const Render::Common::Skin::IdType& skinId);
    std::unordered_map<Render::Common::Skin::IdType, std::array<glm::mat4, 32>> _jointTransforms;

    int _width, _height;

    // Debugging, testing ... seeing if works.
    Render::DebugUtils::TextureProgram _textureDrawProgram;
    Render::EdgeDetector::Program _edgeProgram;

    void BasePass();
};

} // namespace SceneDrawing