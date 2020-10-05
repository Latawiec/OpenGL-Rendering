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

void Program::Draw(const Camera& camera, const Transformed<Model&>& model) const
{
    _program.use();
    prepareCamera(camera);
    prepareTextures();
    prepareUniforms(model);
    VertexDataBase::ScopedBinding bind(model.get().getVertexData());
    glDrawElements(GL_TRIANGLES, model.get().getVertexData().vertexCount(), GL_UNSIGNED_INT, 0);
}

void Program::prepareCamera(const Camera& camera) const
{
    _program.set(u_view, camera.getViewTransform());
    _program.set(u_projection, camera.getProjectionTransform());
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