#include "../SceneDrawing/LightingPass/LightingPassBuffer.hpp"

namespace Renderer {
namespace SceneDrawing {
namespace LightingPass {

LightingPassBuffer::LightingPassBuffer(unsigned int width, unsigned int height)
: FramebufferBase(width, height)
{
    const auto binding = this->Bind();
    glGenTextures(Output::SIZE, outputTextures_);

    // Setup all textures
    [this]<std::size_t ... I>(std::index_sequence<I...>){
        (setupTexture<static_cast<Output>(I)>(), ...);
    }(std::make_index_sequence<Output::SIZE>{});

    const GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(sizeof(bufs)/sizeof(decltype(bufs[0])), bufs);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int LightingPassBuffer::getTexture(const LightingPassBuffer::Output texture) {
    return outputTextures_[texture];
}

template<>
void LightingPassBuffer::setupTexture<LightingPassBuffer::Output::Diffuse>() {
    const auto outputTexture = outputTextures_[Output::Diffuse];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);
}

template<>
void LightingPassBuffer::setupTexture<LightingPassBuffer::Output::Specular>() {
    const auto outputTexture = outputTextures_[Output::Specular];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, outputTexture, 0);
}

} // namespace LightingPass
} // namespace SceneDrawing
} // namespace Renderer
