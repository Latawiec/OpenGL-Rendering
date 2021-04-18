#pragma once

#include <MappingProgram/Animated.hpp>
#include <Scene/Transformed.hpp>
#include <Scene/Scene.hpp>

#include <unordered_map>
#include <cctype>

namespace Render {
namespace SceneDrawing {

struct SkinnedMeshDrawer {

    SkinnedMeshDrawer(const Common::Scene& scene);

    void Draw(const glm::mat4& view,
        const glm::mat4& proj,
        const std::unordered_map<Common::Node::IdType, glm::mat4>& nodeGlobalTransforms);

private:
    void prepareSkin(const Common::Skin::IdType& skinId);

    const Common::Scene& _scene;
    std::unordered_map<Common::Skin::IdType, std::array<glm::mat4, 32>> _jointTransforms;

    // Programs
    MappingProgram::Animated _program;
};

} // Pipeline
} // Render