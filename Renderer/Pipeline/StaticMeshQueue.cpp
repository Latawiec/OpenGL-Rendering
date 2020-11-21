#include "Pipeline/StaticMeshQueue.hpp"

namespace Render {
namespace Pipeline {

StaticMeshQueue::StaticMeshQueue(const Common::Scene& scene) 
: _scene(scene)
{}

void StaticMeshQueue::Queue(const Common::NodeLink& nodeLink)
{
    _queue.emplace_back(nodeLink.GetNode(), nodeLink.GetMesh());
}

void StaticMeshQueue::Draw(const NodeUpdater& nodeUpdater);
{
    const auto& activeCameraId = _scene.GetActiveCamera();
    const auto& activeCamera = _scene.GetCamera(activeCameraId);
    const auto& view = nodeUpdater.GetCameraTransforms()[activeCameraId];
    const auto& proj = activeCamera.GetProjectionTransform();

    const auto binding = _program.Bind();
    _program.SetView(view);
    _program.SetProjection(proj);
    for (const auto& [nodeId, meshId] : _queue) {
        const auto& model = nodeUpdater.GetNodeTransforms()[nodeId];
        _program.SetModel(model);
        _program.Draw(_scene.GetMesh(meshId));
    }
}

} // Pipeline
} // Render