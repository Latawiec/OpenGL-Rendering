#include "Pipeline/NodeUpdater.hpp"

namespace Render {
namespace Pipeline {

NodeUpdater::NodeUpdater(Common::Scene& scene)
: _scene(scene)
{}

void NodeUpdater::Update()
{
    const glm::mat4 initialTransform{1};
    const auto& rootLink = _scene.GetNodeHierarchy();
    updateLink(initialTransform, rootLink);
}

const NodeIdTransformMap GetNodeTransforms() const
{
    return _globalNodeTransforms;
}

const CameraIdTransformMap GetCameraTransforms() const
{
    return _globalCameraTransforms;
}

void NodeUpdater::updateLink(const glm::mat4& parentTransform, NodeLink& link)
{
    const Common::Node& linkNode = scene.GetNode(link.GetNode());
    const glm::mat4 transform =  parentTransform * linkNode.GetTransform();
    _globalNodeTransforms[link.GetNode(), transform];

    // Hacked in here for now...
    if (linkNode.HasProperty(Common::NodeLink::Properties::CAMERA)) {
        const auto& camera = _scene.GetCamera(link.GetCamera());
        _globalCameraTransforms[link.GetCamera(), camera.GetCameraOrientation()];
    }

    for (auto& linkChild : link.GetChildren()) {
        updateLink(transform, linkChild);
    }
}

} // namespace Pipeline
} // namespace Render