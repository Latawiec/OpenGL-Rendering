#pragma once

#include <string_view>

namespace Render {
namespace MappingProgram {
namespace Internal {

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
// Skinning
constexpr static std::string_view u_joint_transform = "jointTransform";

} // Internal
} // MappingProgram
} // Render