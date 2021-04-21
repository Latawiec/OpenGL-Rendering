#include "../SceneDrawing/FramebufferBase.hpp"

#include <utility>

namespace Renderer {
namespace SceneDrawing {

FramebufferBase::FramebufferBase(unsigned int width, unsigned int height)
: _width(width),
  _height(height) {
    glGenFramebuffers(1, &_framebufferId);
}

FramebufferBase::~FramebufferBase() {
    if (_framebufferId != -1) {
        glDeleteFramebuffers(1, &_framebufferId);
    }
}

FramebufferBase::ScopedBinding FramebufferBase::Bind() const {
    return { *this };
}

FramebufferBase::FramebufferBase(FramebufferBase&& other) {
    std::swap(this->_width, other._width);
    std::swap(this->_height, other._height);
    std::swap(this->_framebufferId, other._framebufferId);
}

FramebufferBase& FramebufferBase::operator=(FramebufferBase&& other) {
    std::swap(this->_width, other._width);
    std::swap(this->_height, other._height);
    std::swap(this->_framebufferId, other._framebufferId);

    return *this;
}

FramebufferBase::ScopedBinding::ScopedBinding(const FramebufferBase& framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer._framebufferId);
    glViewport(0, 0, framebuffer._width, framebuffer._height);
}

FramebufferBase::ScopedBinding::~ScopedBinding() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Renderer
} // namespace SceneDrawing