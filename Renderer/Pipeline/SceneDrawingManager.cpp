#include "Pipeline/SceneDrawingManager.hpp"

#include <glad/glad.h>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

namespace Render {
namespace Pipeline {

SceneDrawingManager::SceneDrawingManager(const Common::Scene& scene, const int windowWidth, const int windowHeight)
: _scene(scene)
, _transformProcessor(_scene)
, _staticMeshDrawer(_scene)
, _skinnedMeshDrawer(_scene)
, _deferredBuffers(windowWidth/2, windowHeight/2)
, _width(windowWidth)
, _height(windowHeight) {}

void SceneDrawingManager::Draw() {
    _transformProcessor.Update();

    // Get any camera for now...
    const auto& [nodeId, cameraId] = *_scene.GetCameraNodes().begin();
    const auto& camera = _scene.GetCamera(cameraId);
    const auto viewTransform = glm::inverse(_transformProcessor.GetNodeTransforms().at(nodeId) * camera.GetCameraOrientation());
    const auto projTransform = camera.GetProjectionTransform();

    {
        Common::FramebufferBase::ScopedBinding bind(_deferredBuffers);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _staticMeshDrawer.Draw(viewTransform, projTransform, _transformProcessor.GetNodeTransforms());
        _skinnedMeshDrawer.Draw(viewTransform, projTransform, _transformProcessor.GetNodeTransforms());
    }
    {
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, _width, _height);
        // _edgeProgram.SetImageSize(_width/2, _height/2);
        // _edgeProgram.Draw(_deferredBuffers.getTexture(GraphicBuffer::Output::EdgeInfo));
        _textureDrawProgram.draw(_deferredBuffers.getTexture(GraphicBuffer::Output::Position));
    }
}

} // namespace Pipeline
} // namespace Render