#include "EdgeDetector/Program.hpp"

#include <glad/glad.h>

#include "read_file.hpp"

#ifndef EDGE_DETECTOR_PROGRAM_DIR
#define EDGE_DETECTOR_PROGRAM_DIR "INCORRECT SOURCE DIR"
#endif

namespace Render {
namespace EdgeDetector {

namespace /* anonymous */ {

} // namespace anonymous

Program::Program() {
    auto indicesVector = std::vector<unsigned int>(indices, indices + 6);
    _vertexData = VertexData<Layout::Sequential, Vec3>(indicesVector, 4, reinterpret_cast<const float*>(quadCoords));

    const auto vertexShaderCode = Utils::readFile(EDGE_DETECTOR_PROGRAM_DIR "/edgeDetector.vert.glsl");
    const auto fragmentShaderCode = Utils::readFile(EDGE_DETECTOR_PROGRAM_DIR "/edgeDetector.frag.glsl");

    _program = ShaderProgram(
        Shader<ShaderType::Vertex>(vertexShaderCode.c_str()),
        Shader<ShaderType::Fragment>(fragmentShaderCode.c_str())
    );
}

void Program::Draw(const unsigned int& texture) const {
    _program.use();

    prepareTextures(texture);
    prepareUniforms();

    ScopedBinding bind(_vertexData);
    glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
}

void Program::SetImageSize(const float& width, const float& height) {
    _imageWidth = width;
    _imageHeight = height;
}

void Program::prepareTextures(const unsigned int& texture) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    _program.set(u_edgeMapTexture, 0);
}

void Program::prepareUniforms() const {
    _program.set(u_imageSize, glm::vec2{ _imageWidth, _imageHeight });
}

} // namespace EdgeDetector
} // namespace Render