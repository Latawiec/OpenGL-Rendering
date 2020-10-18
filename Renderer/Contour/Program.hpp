#pragma once

#include <string_view>

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

namespace Render {
namespace Contour {

class Program {

    const ShaderProgram _program;
    // Camera properties
    constexpr static std::string_view u_model = "model";
    constexpr static std::string_view u_view = "view";
    constexpr static std::string_view u_projection = "proj";
    // Textures
    constexpr static std::string_view u_diffuse_texture = "diffuse_tex";
    constexpr static std::string_view u_normal_texture = "normal_tex";
    constexpr static std::string_view u_specular_texture = "specular_tex";
    // Unique value per-mesh to easier define borders between objects
    constexpr static std::string_view u_mesh_id = "mesh_id";
    // Preparation - set uniforms and other important things.
    void prepareCamera(const glm::mat4& view, const glm::mat4& projection) const;
    void prepareTextures() const;
    void prepareUniforms(const glm::mat4& transform) const;

public:
    Program();

    void Draw(const glm::mat4& viewTransform,
              const glm::mat4& projectionTransform,
              const glm::mat4& modelTransform,
              const Mesh& model) const;

    void Draw(const glm::mat4& viewTransform,
              const glm::mat4& projectionTransform,
              const std::vector<std::pair<glm::mat4, const Mesh&>>& transformedModels) const;

    Program(Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(Program&) = delete;
    Program& operator=(Program&&) = delete;
};

} // namespace Contour
} // namespace Render 