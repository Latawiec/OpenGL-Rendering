#pragma once

#include <string_view>

#include "Common/Mesh.hpp"
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
    // Lighting
    constexpr static std::string_view u_light_space_matrix = "lightSpaceMatrix";
    constexpr static std::string_view u_shadow_map_texture = "shadowMap";
    constexpr static std::string_view u_light_position = "lightPosition";

    // Preparation - set uniforms and other important things.
    void prepareCamera(const glm::mat4& view, const glm::mat4& projection) const;
    void prepareTextures() const;
    void prepareUniforms(const glm::mat4& transform) const;

    glm::mat4 _lightSpaceMatrix = glm::mat4(1);
    glm::vec3 _lightPosition = glm::vec3(0);
    GLuint _shadowMapTexture = -1;

public:
    Program();

    void Draw(const glm::mat4& viewTransform,
              const glm::mat4& projectionTransform,
              const glm::mat4& modelTransform,
              const Common::Mesh& model) const;

    void Draw(const glm::mat4& viewTransform,
              const glm::mat4& projectionTransform,
              const std::vector<std::pair<glm::mat4, const Common::Mesh&>>& transformedModels) const;

    void SetLightSpace(const glm::mat4& lightView, const glm::mat4& lightProj);

    void SetShadowMapTexture(const GLuint textureId);

    Program(Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(Program&) = delete;
    Program& operator=(Program&&) = delete;
};

} // namespace Contour
} // namespace Render 