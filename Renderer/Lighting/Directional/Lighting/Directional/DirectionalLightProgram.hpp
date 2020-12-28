#pragma once

#include "Common/Mesh.hpp"
#include "ShaderProgram.hpp"

namespace Render::Lighting::Directional {

class Program {

    const ShaderProgram _program;

public:
    Program();

    void Draw(const Common::Mesh& model) const;

    [[nodiscard("RAII variable discarded.")]]
    ShaderProgram::ScopedBinding Bind();
    void SetView(const glm::mat4& transform) const;
    void SetProjection(const glm::mat4& transform) const;
    void SetModel(const glm::mat4& transform) const;

    Program(Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(Program&) = delete;
    Program& operator=(Program&&) = delete;
};

} // namespace Render::Lighting::Directional