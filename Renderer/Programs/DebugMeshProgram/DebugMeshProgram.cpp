#pragma once 

#include "DebugMeshProgram.hpp"
#include "Base/ShaderCompiler.hpp"
#include "Base/UniformValue.hpp"
#include "glad/glad.h"

#include <read_file.hpp>

#ifndef DEBUG_MESH_PROGRAM_DIR
#define DEBUG_MESH_PROGRAM_DIR "INCORRECT SOURCE DIR"
#endif

namespace Renderer {
namespace Programs {

DebugMeshProgram::DebugMeshProgram() {

    Base::ShaderData<Base::ShaderType::Vertex> vertexShaderData(DEBUG_MESH_PROGRAM_DIR "/debugMesh.vert.glsl");
    Base::ShaderData<Base::ShaderType::Fragment> fragmentShaderData(DEBUG_MESH_PROGRAM_DIR "/debugMesh.frag.glsl");

    _vertexProgram = Base::Compile(vertexShaderData);
    _fragmentProgram = Base::Compile(fragmentShaderData);

    glGenProgramPipelines(1, &_pipeline);

    glUseProgramStages(_pipeline, GL_VERTEX_SHADER_BIT, _vertexProgram.GetId());
    glUseProgramStages(_pipeline, GL_FRAGMENT_SHADER_BIT, _fragmentProgram.GetId());

#ifndef NDEBUG
{
    glValidateProgramPipeline(_pipeline);
    int success;
    glGetProgramPipelineiv(_pipeline, GL_VALIDATE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramPipelineInfoLog(_pipeline, 512, NULL, infoLog);
        std::cerr << "ERROR::PIPELINE::FAIL\n" << infoLog << std::endl;
    }
}
#endif
}

DebugMeshProgram::~DebugMeshProgram() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
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
        glUseProgram(0);
        glBindProgramPipeline(_pipeline);

        prepareUniforms();

        Scene::Base::VertexDataBase::ScopedBinding dataBinding{ vertexData };
        glDrawElements(GL_TRIANGLES, vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
        glBindProgramPipeline(0);
    }
}

void DebugMeshProgram::prepareUniforms() const {
    Base::UniformValue<Base::UniformType::Mat4>(_vertexProgram.GetId(), u_viewTransform).Set(_view);
    Base::UniformValue<Base::UniformType::Mat4>(_vertexProgram.GetId(), u_projectionTransform).Set(_proj);
    Base::UniformValue<Base::UniformType::Mat4>(_vertexProgram.GetId(), u_modelTransform).Set(_model);
    Base::UniformValue<Base::UniformType::Vec3>(_fragmentProgram.GetId(), u_color).Set(_color);
}

} // namespace Programs
} // namespace Renderer