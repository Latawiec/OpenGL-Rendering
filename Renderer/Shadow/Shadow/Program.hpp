#pragma once

#include "ShaderProgram.hpp"
#include "Common/Mesh.hpp"

namespace Render {
namespace Shadow {

class Program {
    const ShaderProgram _program;

    // Uniforms
    constexpr static std::string_view u_model = "model";
    constexpr static std::string_view u_view = "view";
    constexpr static std::string_view u_projection = "proj";

    void prepareLightTransform(const glm::mat4& view, const glm::mat4& projection) const;
    void prepareMeshTransform(const glm::mat4& transform) const;

public:
    Program();

    void Draw(const glm::mat4& viewTransform,
              const glm::mat4& projectionTransform,
              const glm::mat4& modelTransform,
              const Common::Mesh& model) const;

    void Draw(const glm::mat4& viewTransform,
              const glm::mat4& projectionTransform,
              const std::vector<std::pair<glm::mat4, const Common::Mesh&>>& transformedModels) const;
};

} // namespace Shadow
} // namespace Render