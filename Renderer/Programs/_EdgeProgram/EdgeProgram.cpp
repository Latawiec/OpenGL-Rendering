#include "EdgeProgram.hpp"

#include <glad/glad.h>

#include <read_file.hpp>

#ifndef EDGE_PROGRAM_DIR
#define EDGE_PROGRAM_DIR "INCORRECT SOURCE DIR"
#endif

namespace Renderer {
namespace Programs {

namespace /* anonymous */ {

} // namespace anonymous

EdgeProgram::EdgeProgram() {
    auto indicesVector = std::vector<unsigned int>(indices, indices + 6);
    _vertexData = Scene::Base::VertexData<Scene::Base::Layout::Sequential, Scene::Base::Vec3>(indicesVector, 4, reinterpret_cast<const float*>(quadCoords));

    const auto vertexShaderCode = Utils::readFile(EDGE_PROGRAM_DIR "/edgeDetector.vert.glsl");
    const auto fragmentShaderCode = Utils::readFile(EDGE_PROGRAM_DIR "/edgeDetector.frag.glsl");

    _program = Base::ShaderProgram(
        Base::Shader<Base::ShaderType::Vertex>(vertexShaderCode.c_str()),
        Base::Shader<Base::ShaderType::Fragment>(fragmentShaderCode.c_str())
    );
}

void EdgeProgram::Draw(const unsigned int& texture) const {
    Base::ShaderProgram::ScopedBinding programBinding{ _program };

    prepareTextures(texture);
    prepareUniforms();

    Scene::Base::VertexDataBase::ScopedBinding dataBinding{ _vertexData };
    glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
}

void EdgeProgram::SetImageSize(const float& width, const float& height) {
    _imageWidth = width;
    _imageHeight = height;
}

void EdgeProgram::prepareTextures(const unsigned int& texture) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    _program.set(u_edgeMapTexture, 0);
}

void EdgeProgram::prepareUniforms() const {
    _program.set(u_imageSize, glm::vec2{ _imageWidth, _imageHeight });
}

} // namespace Programs
} // namespace Renderer 