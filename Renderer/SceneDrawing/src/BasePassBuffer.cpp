#include "../SceneDrawing/BasePass/BasePassBuffer.hpp"

namespace Renderer {
namespace SceneDrawing {
namespace BasePass {

BasePassBuffer::BasePassBuffer(unsigned int width, unsigned int height)
: FramebufferBase(width, height)
{
    const auto binding = this->Bind();
    glGenTextures(Output::SIZE, outputTextures_);

    // Setup all textures
    [this]<std::size_t ... I>(std::index_sequence<I...>){
        (setupTexture<static_cast<Output>(I)>(), ...);
    }(std::make_index_sequence<Output::SIZE>{});

    // Set attachments for all but Depth. I'll do it manually for now...
    const GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
    glDrawBuffers(sizeof(bufs)/sizeof(decltype(bufs[0])), bufs);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int BasePassBuffer::getTexture(const BasePassBuffer::Output texture) {
    return outputTextures_[texture];
}

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Position>() {
    const auto outputTexture = outputTextures_[Output::Position];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_UNSIGNED_SHORT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);
}

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Albedo>() {
    const auto outputTexture = outputTextures_[Output::Albedo];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, outputTexture, 0);
}

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Normals>() {
    const auto outputTexture = outputTextures_[Output::Normals];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16_SNORM, _width, _height, 0, GL_RGB, GL_UNSIGNED_SHORT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, outputTexture, 0);
}

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::MetallicRoughness>() {
    const auto outputTexture = outputTextures_[Output::MetallicRoughness];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, outputTexture, 0);
}

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::SilhouetteMap>() {
    const auto outputTexture = outputTextures_[Output::SilhouetteMap];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, outputTexture, 0);
}

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Dither>() {
    const auto outputTexture = outputTextures_[Output::Dither];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, _width, _height, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, outputTexture, 0);
}

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Depth>() {
    const auto outputTexture = outputTextures_[Output::Depth];
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, outputTexture, 0);
}

} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer