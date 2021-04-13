#pragma once

#include <Common/Scene.hpp>

#include <glm/glm.hpp>
#include <unordered_map>

namespace Render {
namespace Pipeline {

struct TransformProcessor {
    using NodeIdTransformMap = std::unordered_map<Common::Scene::Node::IdType, glm::mat4>;

    TransformProcessor(const Common::Scene& scene);

    void Update();
    const NodeIdTransformMap& GetNodeTransforms() const;

private:
    void updateLink(const glm::mat4& parentTransform, const Common::NodeLink& link);
    const Common::Scene& _scene;
    NodeIdTransformMap _globalNodeTransforms;
};

} // namespace Pipeline
} // namespace Render