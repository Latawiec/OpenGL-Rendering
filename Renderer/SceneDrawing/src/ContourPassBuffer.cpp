#include "../SceneDrawing/ContourPass/ContourPassBuffer.hpp"

namespace Renderer {
namespace SceneDrawing {
namespace ContourPass {

ContourPassBuffer::ContourPassBuffer(unsigned int width, unsigned int height)
: FramebufferBase(width, height)
{
    const auto binding = this->Bind();
    glGenTextures(Output::SIZE, outputTextures_);

    // Setup all textures
    [this]<std::size_t ... I>(std::index_sequence<I...>){
        (setupTexture<static_cast<Output>(I)>(), ...);
    }(std::make_index_sequence<Output::SIZE>{});

    // Set attachments for all but Depth. I'll do it manually for now...
    const GLenum bufs[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(sizeof(bufs)/sizeof(decltype(bufs[0])), bufs);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int ContourPassBuffer::getTexture(const ContourPassBuffer::Output texture) {
    return outputTextures_[texture];
}

template<>
void ContourPassBuffer::setupTexture<ContourPassBuffer::Output::ContourMap>() {
    const auto outputTexture = outputTextures_[Output::ContourMap];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);
}

} // namespace ContourPass
} // namespace SceneDrawing
} // namespace Renderer