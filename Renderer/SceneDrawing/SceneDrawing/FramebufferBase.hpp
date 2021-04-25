#pragma once

#include <glad/glad.h>

namespace Renderer {
namespace SceneDrawing {

class FramebufferBase {

    struct ScopedBinding {
        ScopedBinding(const FramebufferBase& framebuffer);
        ~ScopedBinding();
    };

public:
    FramebufferBase(unsigned int width, unsigned int height);
    ~FramebufferBase();

    FramebufferBase(FramebufferBase&& other);
    FramebufferBase& operator=(FramebufferBase&& other);

    ScopedBinding Bind() const;

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

protected:
    unsigned int _width, _height;

private:
    unsigned int _framebufferId = -1;
    FramebufferBase(const FramebufferBase&) = delete;
    FramebufferBase& operator=(const FramebufferBase&) = delete;
};

} // namespace Renderer
} // namespace SceneDrawing