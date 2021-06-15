#pragma once 

#include "TextureProgram.hpp"
#include "Base/ShaderCompiler.hpp"
#include "Base/UniformValue.hpp"
#include "glad/glad.h"

#include <read_file.hpp>

#ifndef TEXTURE_PROGRAM_DIR
#define TEXTURE_PROGRAM_DIR "INCORRECT SOURCE DIR"
#endif

namespace Renderer {
namespace Programs {

TextureProgram::TextureProgram() {
    auto indicesVector = std::vector<unsigned int>(indices, indices + 6);
    _vertexData = Scene::Base::VertexData<Scene::Base::Layout::Sequential, Scene::Base::Vec3>(indicesVector, 4, reinterpret_cast<const float*>(quadCoords));

    Base::ShaderData<Base::ShaderType::Vertex> vertexShaderData(TEXTURE_PROGRAM_DIR "/texture.vert.glsl");
    Base::ShaderData<Base::ShaderType::Fragment> fragmentShaderData(TEXTURE_PROGRAM_DIR "/texture.frag.glsl");

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

TextureProgram::~TextureProgram() {
    if (_pipeline != -1) {
        glDeleteProgramPipelines(1, &_pipeline);
    }
}

void TextureProgram::draw(unsigned int texture) const {
    glUseProgram(0);
    glBindProgramPipeline(_pipeline);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    Base::UniformValue<Base::UniformType::Sampler2D>(_fragmentProgram.GetId(), processedTextureName).Set(0);

    Scene::Base::VertexDataBase::ScopedBinding dataBinding{ _vertexData };
    glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
    glBindProgramPipeline(0);
}

} // namespace Programs
} // namespace Renderer