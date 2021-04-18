#pragma once

#include "FramebufferBase.hpp"

namespace Renderer {
namespace SceneDrawing {

class DepthBuffer : public FramebufferBase {
    unsigned int _depthTexture = -1;

    DepthBuffer(const DepthBuffer& other) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;
public:
    DepthBuffer(unsigned int width, unsigned int height);
    ~DepthBuffer();

    unsigned int getTexture() const;

};

} // namespace Renderer
} // namespace SceneDrawing