#pragma once 

#include "TextureProgram.hpp"
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

    const auto vertexShaderCode = Utils::readFile(TEXTURE_PROGRAM_DIR "/texture.vert.glsl");
    const auto fragmentShaderCode = Utils::readFile(TEXTURE_PROGRAM_DIR "/texture.frag.glsl");

    _shaderProgram = Base::ShaderProgram(
        Base::Shader<Base::ShaderType::Vertex>(vertexShaderCode.c_str()),
        Base::Shader<Base::ShaderType::Fragment>(fragmentShaderCode.c_str())
    );
}

void TextureProgram::draw(unsigned int texture) const {
    Base::ShaderProgram::ScopedBinding programBinding{ _shaderProgram };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    _shaderProgram.set(processedTextureName, 0);

    Scene::Base::VertexDataBase::ScopedBinding dataBinding{ _vertexData };
    glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
}

} // namespace Programs
} // namespace Renderer