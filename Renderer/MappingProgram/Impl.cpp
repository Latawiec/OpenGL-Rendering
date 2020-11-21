#include "MappingProgram/Animated.hpp"
#include "MappingProgram/Static.hpp"

#include "UniformDefinitions.hpp"
#include "read_file.hpp"

#ifndef MAPPING_PROGRAM_DIR
#define MAPPING_PROGRAM_DIR "Path not provided."
#endif

namespace Render {
namespace MappingProgram {
namespace /*anonymous*/ {

// Unique-id will just be a monotonic counter.
// I don't suspect to have over 65545 objects in viewport.
static std::uint16_t id_counter = 0;
static std::uint16_t generateId() {
    return id_counter++;
}

} // namespace anonymous


// Static

Static::Static()
: _program (
    Shader<ShaderType::Vertex>(Utils::readFile(MAPPING_PROGRAM_DIR "/MappingProgram.vert.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(MAPPING_PROGRAM_DIR "/MappingProgram.frag.glsl").c_str())
)
{}

void Static::Draw(const Common::Mesh& mesh) const
{
    _program.set<unsigned int>(Internal::u_mesh_id, generateId());
    VertexArrayBase::ScopedBinding bind(mesh.getVertexData());
    glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
}

void Static::SetView(const glm::mat4& transform) const 
{
    _program.set(Internal::u_view, transform);
}

void Static::SetProjection(const glm::mat4& transform) const 
{
    _program.set(Internal::u_projection, transform);
}

void Static::SetModel(const glm::mat4& transform) const
{
    _program.set(Internal::u_model, transform);
}

// Animated

Animated::Animated()
: _program (
    Shader<ShaderType::Vertex>(Utils::readFile(MAPPING_PROGRAM_DIR "/MappingProgram.vert.animated.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(MAPPING_PROGRAM_DIR "/MappingProgram.frag.glsl").c_str())
)
{}

void Animated::Draw(const Common::Mesh& mesh) const
{
    _program.set<unsigned int>(Internal::u_mesh_id, generateId());
    VertexArrayBase::ScopedBinding bind(mesh.getVertexData());
    glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
}

void Animated::SetView(const glm::mat4& transform) const 
{
    _program.set(Internal::u_view, transform);
}

void Animated::SetProjection(const glm::mat4& transform) const 
{
    _program.set(Internal::u_projection, transform);
}

void Animated::SetModel(const glm::mat4& transform) const
{
    _program.set(Internal::u_model, transform);
}

void Animated::SetJoints(const JointsArray& transforms) const 
{
    _program.setArray(Internal::u_joint_transform, transforms.size(), transforms.data());
}

} // namespace MappingProgram
} // namespace Render