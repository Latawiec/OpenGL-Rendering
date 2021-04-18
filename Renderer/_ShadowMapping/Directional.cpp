#include "Directional/Directional.hpp"

#ifndef DIRECTIONAL_SHADOW_PROGRAM_DIR
#define DIRECTIONAL_SHADOW_PROGRAM_DIR "Path not provided."
#endif

namespace Render {
namespace ShadowMapping {
namespace Directional {

// NON ANIMATED
Static::Static()
: _program (
    Shader<ShaderType::Vertex>(Utils::readFile(DIRECTIONAL_SHADOW_PROGRAM_DIR "/Directional.vert.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(DIRECTIONAL_SHADOW_PROGRAM_DIR "/Directional.frag.glsl").c_str())
)

void Static::Draw(const Common::Mesh& mesh) const 
{
    VertexDataBase::ScopedBinding dataBinding{ mesh.getVertexData() };
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

ShaderProgram::ScopedBinding Static::Bind()
{
    return ShaderProgram::ScopedBinding(_program);
}

// ANIMATED
Animated::Animated()
: _program (
    Shader<ShaderType::Vertex>(Utils::readFile(DIRECTIONAL_SHADOW_PROGRAM_DIR "/Directional.vert.animated.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(DIRECTIONAL_SHADOW_PROGRAM_DIR "/Directional.frag.glsl").c_str())
)
{}

void Animated::Draw(const Common::Mesh& mesh) const
{
    VertexDataBase::ScopedBinding dataBinding{ mesh.getVertexData() };
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

ShaderProgram::ScopedBinding Animated::Bind()
{
    return ShaderProgram::ScopedBinding(_program);
}

} // namespace Directional
} // namespace ShadowMapping
} // namespace Render