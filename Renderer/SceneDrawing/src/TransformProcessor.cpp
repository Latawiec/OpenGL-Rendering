#include "SceneDrawing/TransformProcessor.hpp"

namespace Renderer {
namespace SceneDrawing {

TransformProcessor::TransformProcessor(const Renderer::Scene::Scene& scene)
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

void TransformProcessor::updateLink(const glm::mat4& parentTransform, const Renderer::Scene::NodeLink& link)
{
    const Renderer::Scene::Base::Node& linkNode = _scene.GetNode(link.GetNode());
    const glm::mat4 transform =  linkNode.GetTransform() * parentTransform;
    _globalNodeTransforms[link.GetNode()] = transform;

    for (auto& linkChild : link.GetChildren()) {
        updateLink(transform, linkChild);
    }
}

} // namespace Renderer
} // namespace SceneDrawing
