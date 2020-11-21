#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <Common/IdGenerator.hpp>
#include <Common/Scene.hpp>
#include <Common/NodeLink.hpp>
#include <Common/Transformed.hpp>
#include <MappingProgram/Static.hpp>
#include <MappingProgram/Animated.hpp>
#include <EdgeDetector/Program.hpp>
#include "GraphicBuffer.hpp"
#include "TransformProcessor.hpp"
#include "StaticMeshDrawer.hpp"
#include "SkinnedMeshDrawer.hpp"

#include "TextureProgram.hpp"

namespace Render {
namespace Pipeline {

struct SceneDrawingManager {

SceneDrawingManager(const Common::Scene& scene, const int windowWidth, const int windowHeight);

void Draw();

private:
    const Common::Scene& _scene;

    GraphicBuffer _deferredBuffers;
    TransformProcessor _transformProcessor;
    StaticMeshDrawer _staticMeshDrawer;
    SkinnedMeshDrawer _skinnedMeshDrawer;
    int _width, _height;

    // Debugging, testing ... seeing if works.
    DebugUtils::TextureProgram _textureDrawProgram;
    EdgeDetector::Program _edgeProgram;
};

} // namespace Pipeline
} // namespace Render