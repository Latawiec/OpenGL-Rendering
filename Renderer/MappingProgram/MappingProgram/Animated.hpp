#pragma once

#include "Scene/Mesh.hpp"
#include "ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <array>

namespace Render {
namespace MappingProgram {

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

} // namespace MappingProgram
} // namespace Render 
