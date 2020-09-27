#pragma once

#include <glad/glad.h>

namespace Render {

class FramebufferBase {
private:
    unsigned int _framebufferId;
    
protected:
    unsigned int _width, _height;

public:
    FramebufferBase(unsigned int width, unsigned int height) : _width(width), _height(height) {
        glGenFramebuffers(1, &_framebufferId);
    }

    ~FramebufferBase() {
        glDeleteFramebuffers(1, &_framebufferId);
    }

    /**
     * Used to bind framebuffer as render target in scope of this object.
     */
    struct ScopedBinding {
        ScopedBinding(const FramebufferBase& framebuffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer._framebufferId);
            glViewport(0, 0, framebuffer._width, framebuffer._height);
        }

        ~ScopedBinding() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    };

    FramebufferBase(const FramebufferBase&) = delete;
};

} // namespace Render