#include "Pipeline/SceneDrawingManager.hpp"

#include <glad/glad.h>

namespace Render {

SceneDrawingManager::SceneDrawingManager(const int windowWidth, const int windowHeight)
: _deferredBuffers(windowWidth/2, windowHeight/2)
, _width(windowWidth)
, _height(windowHeight) {}

void SceneDrawingManager::Draw(const Common::Scene& scene) {
    // prepareSkins(scene, scene.GetNodeHierarchy());
    matchForQueue(scene, scene.GetNodeHierarchy());
    drawQueues();
}

void SceneDrawingManager::matchForQueue(const Common::Scene& scene, const Common::NodeLink& link) {
    const auto linkProperties = link.GetProperties();
    
    if (link.HasProperties(Common::NodeLink::CONTOUR_MESH)) queueContourMesh(link.GetCachedTransform(), scene.GetMesh(link.GetMesh()));
    if (link.HasProperties(Common::NodeLink::CASTS_SHADOW)) queueShadowMesh(link.GetCachedTransform(), scene.GetMesh(link.GetMesh()));
    if (link.HasProperties(Common::NodeLink::CAMERA)) queueCamera(link.GetCachedTransform(), scene.GetCamera(link.GetCamera()));

    for (const auto& childLink : link.GetChildren()) {
        matchForQueue(scene, childLink);
    }
}

// void SceneDrawingManager::prepareSkins(const Common::Scene& scene, const Common::NodeLink& link) {

// }

void SceneDrawingManager::queueContourMesh(const glm::mat4& transform, const Common::Mesh& mesh) {
    _contourProgramExecutor.QueueMesh(transform, mesh);
}

void SceneDrawingManager::queueShadowMesh(const glm::mat4& transform, const Common::Mesh& mesh) {
    // we'll get there!
}

void SceneDrawingManager::queueCamera(const glm::mat4& worldTransform, const Common::Camera& camera) {
    _activeCamera = {
        glm::inverse(worldTransform * camera.GetCameraOrientation()),
        camera.GetProjectionTransform()
    };
}

void SceneDrawingManager::drawQueues() {
    {
        Common::FramebufferBase::ScopedBinding bind(_deferredBuffers);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _contourProgramExecutor.Draw(_activeCamera.viewTransform, _activeCamera.projectionTransform);
    }

    // Testing
    {
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, _width, _height);
        _edgeProgram.SetImageSize(_width/2, _height/2);
        _edgeProgram.Draw(_deferredBuffers.getTexture(GraphicBuffer::Output::EdgeInfo));
        //_textureDrawProgram.draw(_deferredBuffers.getTexture(GraphicBuffer::Output::EdgeInfo));
    }

    _contourProgramExecutor.Clear();
}

} // namespace Render