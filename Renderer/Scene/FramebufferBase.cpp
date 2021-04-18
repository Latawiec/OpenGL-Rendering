#include "Scene/FramebufferBase.hpp"

namespace Render {
namespace Common {

FramebufferBase::FramebufferBase(unsigned int width, unsigned int height)
: _width(width),
  _height(height) {
    glGenFramebuffers(1, &_framebufferId);
}

FramebufferBase::~FramebufferBase() {
    glDeleteFramebuffers(1, &_framebufferId);
}

FramebufferBase::ScopedBinding::ScopedBinding(const FramebufferBase& framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer._framebufferId);
    glViewport(0, 0, framebuffer._width, framebuffer._height);
}

FramebufferBase::ScopedBinding::~ScopedBinding() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Common
} // namespace Render