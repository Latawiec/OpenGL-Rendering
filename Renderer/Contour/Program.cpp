#include "Contour/Program.hpp"
#include "read_file.hpp"

#ifndef CONTOUR_PROGRAM_DIR
#define CONTOUR_PROGRAM_DIR "Path not provided."
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
    Shader<ShaderType::Vertex>(Utils::readFile(CONTOUR_PROGRAM_DIR "/Contour.vert.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(CONTOUR_PROGRAM_DIR "/Contour.frag.glsl").c_str())
)
{}

void Program::Draw(const glm::mat4& viewTransform,
          const glm::mat4& projectionTransform,
          const glm::mat4& meshTransform,
          const Common::Mesh& mesh) const
{
    _program.use();
    prepareCamera(viewTransform, projectionTransform);
    prepareTextures();
    prepareUniforms(meshTransform);
    ScopedBinding bind(mesh.getVertexData());
    glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
}

void Program::Draw(const glm::mat4& viewTransform,
          const glm::mat4& projectionTransform,
          const std::vector<std::pair<glm::mat4, const Common::Mesh&>>& transformedMeshes) const
{
    _program.use();
    prepareCamera(viewTransform, projectionTransform);
    for (const auto& [meshTransform, mesh] : transformedMeshes) {
        prepareTextures();
        prepareUniforms(meshTransform);
        ScopedBinding bind(mesh.getVertexData());
        glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
        while (auto error = glGetError()) {
           std::cerr << "Error: " << error << '\n';
	    }
    }
}

void Program::SetLightSpaceMatrix(const glm::mat4& lightSpaceMatrix) {
    _lightSpaceMatrix = lightSpaceMatrix;
}

void Program::SetShadowMapTexture(const GLuint textureId) {
    _shadowMapTexture = textureId;
}

void Program::prepareCamera(const glm::mat4& view, const glm::mat4& projection) const
{
    _program.set(u_view, view);
    _program.set(u_projection, projection);
}

void Program::prepareTextures() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _shadowMapTexture);
}

void Program::prepareUniforms(const glm::mat4& transform) const
{
    _program.set<unsigned int>(u_mesh_id, generateId());
    _program.set(u_model, transform);
    _program.set(u_light_space_matrix, _lightSpaceMatrix);
}

} // namespace Contour
} // namespace Render