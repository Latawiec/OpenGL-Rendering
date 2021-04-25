#pragma once 

#include "DebugMeshProgram.hpp"
#include "glad/glad.h"

#include <read_file.hpp>

#ifndef DEBUG_MESH_PROGRAM_DIR
#define DEBUG_MESH_PROGRAM_DIR "INCORRECT SOURCE DIR"
#endif

namespace Renderer {
namespace Programs {

DebugMeshProgram::DebugMeshProgram() {

    const auto vertexShaderCode = Utils::readFile(DEBUG_MESH_PROGRAM_DIR "/debugMesh.vert.glsl");
    const auto fragmentShaderCode = Utils::readFile(DEBUG_MESH_PROGRAM_DIR "/debugMesh.frag.glsl");

    _shaderProgram = Base::ShaderProgram(
        Base::Shader<Base::ShaderType::Vertex>(vertexShaderCode.c_str()),
        Base::Shader<Base::ShaderType::Fragment>(fragmentShaderCode.c_str())
    );
}

void DebugMeshProgram::SetMesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
    _vertexData.emplace(indices, vertices.size(), reinterpret_cast<const float*>(vertices.data()));
}

void DebugMeshProgram::SetView(const glm::mat4& view) {
    _view = view;
}

void DebugMeshProgram::SetProjection(const glm::mat4& proj) {
    _proj = proj;
}

void DebugMeshProgram::SetModel(const glm::mat4& model) {
    _model = model;
}

bool DebugMeshProgram::HasMesh() const {
    return static_cast<bool>(_vertexData);
}

void DebugMeshProgram::Draw() const {

    if (_vertexData) {
        const auto& vertexData = *_vertexData;
        Base::ShaderProgram::ScopedBinding programBinding{ _shaderProgram };

        prepareUniforms();

        Scene::Base::VertexDataBase::ScopedBinding dataBinding{ vertexData };
        glDrawElements(GL_TRIANGLES, vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
    }
}

void DebugMeshProgram::prepareUniforms() const {
    _shaderProgram.set(u_viewTransform, _view);
    _shaderProgram.set(u_projectionTransform, _proj);
    _shaderProgram.set(u_modelTransform, _model);
    _shaderProgram.set(u_color, _color);
}

} // namespace Programs
} // namespace Renderer