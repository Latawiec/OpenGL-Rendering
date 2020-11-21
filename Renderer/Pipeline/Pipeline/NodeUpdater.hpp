#pragma once

#include <Common/Scene.hpp>

#include <glm/glm.hpp>
#include <unordered_map>

namespace Render {
namespace Pipeline {

struct NodeUpdater {
    using NodeIdTransformMap = std::unordered_map<Common::Scene::NodeIdType, glm::mat4>;
    using CameraIdTransformMap = std::unordered_map<Common::Scene::CameraIdType, glm::mat4>;

    NodeUpdater(Common::Scene& scene);

    void Update();
    const NodeIdTransformMap GetNodeTransforms() const;
    const CameraIdTransformMap GetCameraTransforms() const;

private:
    void updateLink(const glm::mat4& parentTransform, NodeLink& link);
    Common::Scene& _scene;
    NodeIdTransformMap _globalNodeTransforms;
    CameraIdTransformMap _globalCameraTransforms;
};

} // namespace Pipeline
} // namespace Render