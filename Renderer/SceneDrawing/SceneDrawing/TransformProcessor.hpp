#pragma once

#include <Scene/Scene.hpp>

#include <glm/glm.hpp>
#include <unordered_map>

namespace Renderer {
namespace SceneDrawing {

struct TransformProcessor {
    using NodeIdTransformMap = std::unordered_map<Renderer::Scene::Base::Node::IdType, glm::mat4>;

    TransformProcessor(const Renderer::Scene::Scene& scene);

    void Update();
    const NodeIdTransformMap& GetNodeTransforms() const;

private:
    void updateLink(const glm::mat4& parentTransform, const Renderer::Scene::NodeLink& link);
    const Renderer::Scene::Scene& _scene;
    NodeIdTransformMap _globalNodeTransforms;
};

} // namespace Renderer
} // namespace SceneDrawing