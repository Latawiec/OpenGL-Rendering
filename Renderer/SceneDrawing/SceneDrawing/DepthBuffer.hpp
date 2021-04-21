#pragma once

#include "FramebufferBase.hpp"

namespace Renderer {
namespace SceneDrawing {

struct DepthBuffer : public FramebufferBase {

    DepthBuffer(unsigned int width, unsigned int height);
    ~DepthBuffer();

    DepthBuffer(DepthBuffer&& other);
    DepthBuffer& operator=(DepthBuffer&& other);

    unsigned int getTexture() const;

private:
    unsigned int _depthTexture = -1;

    DepthBuffer(const DepthBuffer& other) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;
};

} // namespace Renderer
} // namespace SceneDrawing