#include "../SceneDrawing/DepthBuffer.hpp"

#include <utility>
#include <glad/glad.h>

namespace Renderer {
namespace SceneDrawing {
namespace /* anonymous */ {

[[nodiscard("Generated texture has to be freed somewhere.")]]
unsigned int prepareDepthTexture(unsigned int width, unsigned int height) {
    unsigned int textureHandle = -1;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    return textureHandle;
}
} // namespace anonymous

DepthBuffer::DepthBuffer(unsigned int width, unsigned int height)
: FramebufferBase(width, height)
, _depthTexture(prepareDepthTexture(width, height)) {
    const auto binding = this->Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

DepthBuffer::~DepthBuffer() {
    if (_depthTexture != -1) {
        glDeleteTextures(1, &_depthTexture);
    }
}

DepthBuffer::DepthBuffer(DepthBuffer&& other) : FramebufferBase(std::move(other)) {
    std::swap(this->_depthTexture, other._depthTexture);
}

DepthBuffer& DepthBuffer::operator=(DepthBuffer&& other) {
    FramebufferBase::operator=(std::move(other));
    std::swap(this->_depthTexture, other._depthTexture);

    return *this;
}

unsigned int DepthBuffer::getTexture() const {
    return _depthTexture;
}

} // namespace Renderer
} // namespace SceneDrawing