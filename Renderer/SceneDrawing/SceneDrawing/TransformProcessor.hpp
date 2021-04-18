#pragma once

#include <Scene/Scene.hpp>

#include <glm/glm.hpp>
#include <unordered_map>

namespace SceneDrawing {

struct TransformProcessor {
    using NodeIdTransformMap = std::unordered_map<Render::Common::Node::IdType, glm::mat4>;

    TransformProcessor(const Render::Common::Scene& scene);

    void Update();
    const NodeIdTransformMap& GetNodeTransforms() const;

private:
    void updateLink(const glm::mat4& parentTransform, const Render::Common::NodeLink& link);
    const Render::Common::Scene& _scene;
    NodeIdTransformMap _globalNodeTransforms;
};

} // namespace SceneDrawing