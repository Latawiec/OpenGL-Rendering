#include "Program.hpp"
#include "read_file.hpp"

#ifndef CONTOUR_PROGRAM
#define CONTOUR_PROGRAM "Path not provided."
#endif

namespace Render {
namespace Contour {
namespace /*anonymous*/ {

// Unique-id will just be a monotonic counter.
// I don't suspect to have over 65545 objects in viewport.
static std::uint16_t id_counter = 0;
static std::uint16_t generateId() {
    return id_counter++;
}

} // namespace anonymous

Program::Program()
: _program (
    Shader<ShaderType::Vertex>(Utils::readFile(CONTOUR_PROGRAM "/Contour.vert.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(CONTOUR_PROGRAM "/Contour.frag.glsl").c_str())
)
{
}

void Program::Draw(const glm::mat4& viewTransform,
          const glm::mat4& projectionTransform,
          const glm::mat4& modelTransform,
          const Model& model) const
{
    _program.use();
    prepareCamera(viewTransform, projectionTransform);
    prepareTextures();
    prepareUniforms(modelTransform);
    ScopedBinding bind(model.getVertexData());
    glDrawElements(GL_TRIANGLES, model.getVertexData().vertexCount(), GL_UNSIGNED_INT, 0);
}

void Program::Draw(const glm::mat4& viewTransform,
          const glm::mat4& projectionTransform,
          const std::vector<std::pair<glm::mat4, const Model&>>& transformedModels) const
{
    _program.use();
    prepareCamera(viewTransform, projectionTransform);
    for (const auto& [modelTransform, model] : transformedModels) {
        prepareTextures();
        prepareUniforms(modelTransform);
        ScopedBinding bind(model.getVertexData());
        glDrawElements(GL_TRIANGLES, model.getVertexData().vertexCount(), GL_UNSIGNED_INT, 0);
    }
}

void Program::prepareCamera(const glm::mat4& view, const glm::mat4& projection) const
{
    _program.set(u_view, view);
    _program.set(u_projection, projection);
}

void Program::prepareTextures() const
{
    // TODO
}

void Program::prepareUniforms(const TransformBase& transform) const
{
    _program.set<unsigned int>(u_mesh_id, generateId());
    _program.set(u_model, transform.get());
}

} // namespace Contour
} // namespace Render