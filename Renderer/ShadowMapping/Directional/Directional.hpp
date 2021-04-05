#pragma once

#include "Common/Mesh.hpp"
#include "ShaderProgram.hpp"

#include <glm/glm.hpp>

namespace Render {
namespace ShadowMapping {
namespace Directional {

/**
 * For drawing shadowmap of non-animated meshes.
 */
class Static {

    const ShaderProgram _program;
    
public:
    Static();

    void Draw(const Common::Mesh& model) const;

    [[nodiscard("RAII variable discarded.")]]
    ShaderProgram::ScopedBinding Bind();
    void SetView(const glm::mat4& transform) const;
    void SetProjection(const glm::mat4& transform) const;
    void SetModel(const glm::mat4& transform) const;

    Static(Static&) = delete;
    Static(Static&&) = delete;
    Static& operator=(Static&) = delete;
    Static& operator=(Static&&) = delete;
};

/**
 * For drawing shadowmap of animated meshes.
 */
class Animated {
    static constexpr int MaxJointsCount = 32;
    using JointsArray = std::array<glm::mat4, MaxJointsCount>;

    const ShaderProgram _program;

public:
    Animated();

    void Draw(const Common::Mesh& model) const;

    [[nodiscard("RAII variable discarded.")]]
    ShaderProgram::ScopedBinding Bind();
    void SetView(const glm::mat4& transform) const;
    void SetProjection(const glm::mat4& transform) const;
    void SetModel(const glm::mat4& transform) const;
    void SetJoints(const JointsArray& transforms) const;

    Animated(Animated&) = delete;
    Animated(Animated&&) = delete;
    Animated& operator=(Animated&) = delete;
    Animated& operator=(Animated&&) = delete;
};

} // namespace Directional
} // namespace ShadowMapping
} // namespace Render