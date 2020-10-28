#include "Shadow/Program.hpp"
#include "read_file.hpp"

#ifndef SHADOW_PROGRAM_DIR
#define SHADOW_PROGRAM_DIR "Path not provided."
#endif

namespace Render {
namespace Shadow {

Program::Program()
: _program (
    Shader<ShaderType::Vertex>(Utils::readFile(SHADOW_PROGRAM_DIR "/Shadow.vert.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(SHADOW_PROGRAM_DIR "/Shadow.frag.glsl").c_str())
)
{}

void Program::Draw(const glm::mat4& viewTransform,
          const glm::mat4& projectionTransform,
          const glm::mat4& meshTransform,
          const Common::Mesh& mesh) const
{
    _program.use();
    prepareLightTransform(viewTransform, projectionTransform);
    prepareMeshTransform(meshTransform);
    ScopedBinding bind(mesh.getVertexData());
    glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
}

void Program::Draw(const glm::mat4& viewTransform,
          const glm::mat4& projectionTransform,
          const std::vector<std::pair<glm::mat4, const Common::Mesh&>>& transformedMeshes) const
{
    _program.use();
    prepareLightTransform(viewTransform, projectionTransform);
    for (const auto& [meshTransform, mesh] : transformedMeshes) {
        prepareMeshTransform(meshTransform);
        ScopedBinding bind(mesh.getVertexData());
        glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
    }
}

void Program::prepareLightTransform(const glm::mat4& view, const glm::mat4& projection) const 
{
    _program.set(u_view, view);
    _program.set(u_projection, projection);
}

void Program::prepareMeshTransform(const glm::mat4& transform) const
{
    _program.set(u_model, transform);
}

} // namespace Shadow
} // namespace Render