#pragma once

#include <MappingProgram/Animated.hpp>
#include <Common/Transformed.hpp>
#include <Common/Scene.hpp>

#include <unordered_map>
#include <cctype>

namespace Render {
namespace Pipeline {

struct SkinnedMeshQueue {

    SkinnedMeshQueue(const Common::Scene& scene);

    void Queue(const Common::NodeLink& nodeLink);
    void Draw(const std::unordered_map<Common::Scene::NodeIdType, glm::mat4>&);

private:
    struct QueuedElement {
        const Common::Scene::NodeIdType;
        const Common::Scene::MeshIdType;
    };
    void prepareSkin(const Common::Scene::SkinIdType& skinId);

    const Common::Scene& _scene;
    std::unordered_multimap<Common::Scene::SkinIdType, QueuedElement> _queue;
    std::unordered_map<Common::Scene::SkinIdType, std::array<glm::mat4, 32>> _jointTransforms;

    // Programs
    MappingProgram::Animated _program;
};

} // Pipeline
} // Render