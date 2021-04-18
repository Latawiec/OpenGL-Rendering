#pragma once

#include "Scene/Mesh.hpp"
#include "ShaderProgram.hpp"

#include <glm/glm.hpp>

namespace Render {
namespace MappingProgram {

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

} // namespace MappingProgram
} // namespace Render 