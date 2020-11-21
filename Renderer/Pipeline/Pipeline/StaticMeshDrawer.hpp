#pragma once

#include <MappingProgram/Static.hpp>
#include <Common/Transformed.hpp>
#include <Common/Scene.hpp>

#include <vector>
#include <cctype>

namespace Render {
namespace Pipeline {

struct StaticMeshDrawer {

    StaticMeshDrawer(const Common::Scene& scene);

    void Draw(const glm::mat4& view,
        const glm::mat4& proj,
        const std::unordered_map<Common::Scene::NodeIdType, glm::mat4>& nodeGlobalTransforms);

private:
    const Common::Scene& _scene;

    // Programs
    MappingProgram::Static _program;
};

} // Pipeline
} // Render