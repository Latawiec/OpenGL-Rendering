#pragma once

#include <MappingProgram/Static.hpp>
#include <Common/Transformed.hpp>
#include <Common/Scene.hpp>

#include <vector>
#include <cctype>

namespace Render {
namespace Pipeline {

struct SkinnedMeshQueue {

    SkinnedMeshQueue(const Common::Scene& scene);

    void Queue(const Common::NodeLink& nodeLink);
    void Draw(const NodeUpdater& nodeUpdater);

private:
    struct QueuedElement {
        const Common::Scene::NodeIdType;
        const Common::Scene::MeshIdType;
    };
    const Common::Scene& _scene;
    std::vector<QueuedElement> _queue;

    // Programs
    MappingProgram::Static _program;
};

} // Pipeline
} // Render