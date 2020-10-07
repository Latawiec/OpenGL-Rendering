#pragma once 

#include "TextureProgram.hpp"
#include "glad/glad.h"

#include "read_file.hpp"

#ifndef SHADERS_SOURCE_DIR
#define SHADERS_SOURCE_DIR "INCORRECT SOURCE DIR"
#endif


namespace Render {


TextureProgram::TextureProgram() {
    auto indicesVector = std::vector<unsigned int>(indices, indices + 6);
    _vertexData = VertexData<Layout::Sequential, Vec3>(indicesVector, 4, reinterpret_cast<const float*>(quadCoords));

    const auto vertexShaderCode = Utils::readFile(SHADERS_SOURCE_DIR "/Deferred/SimpleTexture/" "texture.vert.glsl");
    const auto fragmentShaderCode = Utils::readFile(SHADERS_SOURCE_DIR "/Deferred/SimpleTexture/" "texture.frag.glsl");

    _shaderProgram = ShaderProgram(
        Shader<ShaderType::Vertex>(vertexShaderCode.c_str()),
        Shader<ShaderType::Fragment>(fragmentShaderCode.c_str())
    );
}

void TextureProgram::draw(unsigned int texture) const {
    _shaderProgram.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    _shaderProgram.set(processedTextureName, 0);

    ScopedBinding bind(_vertexData);
    glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
}

} // namespace Render