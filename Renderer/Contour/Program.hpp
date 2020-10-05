#pragma once

#include <string_view>

#include "Model.hpp"
#include "Transform.hpp"
#include <Camera.hpp>
#include <ShaderProgram.hpp>

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
    void prepareCamera(const Camera&) const;
    void prepareTextures() const;
    void prepareUniforms(const TransformBase&) const;

public:
    Program();

    void Draw(const Camera& camera, const Transformed<Model&>& model) const;

    Program(Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(Program&) = delete;
    Program& operator=(Program&&) = delete;
};

} // namespace Contour
} // namespace Render 