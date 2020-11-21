#include "Pipeline/TransformProcessor.hpp"

namespace Render {
namespace Pipeline {

TransformProcessor::TransformProcessor(const Common::Scene& scene)
: _scene(scene)
{}

void TransformProcessor::Update()
{
    const glm::mat4 initialTransform{1};
    const auto& rootLink = _scene.GetNodeHierarchy();
    updateLink(initialTransform, rootLink);
}

const TransformProcessor::NodeIdTransformMap& TransformProcessor::GetNodeTransforms() const
{
    return _globalNodeTransforms;
}

void TransformProcessor::updateLink(const glm::mat4& parentTransform, const Common::NodeLink& link)
{
    const Common::Node& linkNode = _scene.GetNode(link.GetNode());
    const glm::mat4 transform =  parentTransform * linkNode.GetTransform();
    _globalNodeTransforms[link.GetNode()] = transform;

    for (auto& linkChild : link.GetChildren()) {
        updateLink(transform, linkChild);
    }
}

} // namespace Pipeline
} // namespace Render