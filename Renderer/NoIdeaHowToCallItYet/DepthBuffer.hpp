#pragma once

#include "FramebufferBase.hpp"

namespace Render {

class DepthBuffer : public FramebufferBase {
    unsigned int _depthTexture = -1;

    DepthBuffer(const DepthBuffer& other) = delete;
    DepthBuffer& operator=(const DepthBuffer&) = delete;
public:
    DepthBuffer(unsigned int width, unsigned int height);
    ~DepthBuffer();

    unsigned int DepthBuffer::getTexture();

};

} // namespace Render