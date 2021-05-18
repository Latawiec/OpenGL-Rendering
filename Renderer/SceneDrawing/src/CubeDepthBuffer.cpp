#include "../SceneDrawing/CubeDepthBuffer.hpp"

#include <utility>
#include <glad/glad.h>

namespace Renderer {
namespace SceneDrawing {
namespace /* anonymous */ {

[[nodiscard("Generated texture has to be freed somewhere.")]]
unsigned int prepareDepthTexture(unsigned int width, unsigned int height) {
    unsigned int textureHandle = -1;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);
    for (int i=0; i<6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureHandle;
}
} // namespace anonymous

CubeDepthBuffer::CubeDepthBuffer(unsigned int width, unsigned int height)
: FramebufferBase(width, height)
, _cubeDepthTexture(prepareDepthTexture(width, height)) {
    const auto binding = this->Bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _cubeDepthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

CubeDepthBuffer::~CubeDepthBuffer() {
    if (_cubeDepthTexture != -1) {
        glDeleteTextures(1, &_cubeDepthTexture);
    }
}

CubeDepthBuffer::CubeDepthBuffer(CubeDepthBuffer&& other) : FramebufferBase(std::move(other)) {
    std::swap(this->_cubeDepthTexture, other._cubeDepthTexture);
}

CubeDepthBuffer& CubeDepthBuffer::operator=(CubeDepthBuffer&& other) {
    FramebufferBase::operator=(std::move(other));
    std::swap(this->_cubeDepthTexture, other._cubeDepthTexture);

    return *this;
}

unsigned int CubeDepthBuffer::getTexture() const {
    return _cubeDepthTexture;
}

} // namespace SceneDrawing
} // namespace Renderer