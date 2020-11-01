#pragma once

#include <glad/glad.h>

namespace Render {
namespace Common {

class FramebufferBase {
private:
    unsigned int _framebufferId;
    FramebufferBase(const FramebufferBase&) = delete;
    
protected:
    unsigned int _width, _height;

public:
    FramebufferBase(unsigned int width, unsigned int height);
    ~FramebufferBase();

    /**
     * Used to bind framebuffer as render target in scope of this object.
     */
    struct ScopedBinding {
        ScopedBinding(const FramebufferBase& framebuffer);
        ~ScopedBinding();
    };
};

} // namespace Common
} // namespace Render