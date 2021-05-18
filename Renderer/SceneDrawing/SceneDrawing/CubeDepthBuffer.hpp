#pragma once

#include "FramebufferBase.hpp"

namespace Renderer {
namespace SceneDrawing {

struct CubeDepthBuffer : public FramebufferBase {

    CubeDepthBuffer(unsigned int width, unsigned int height);
    ~CubeDepthBuffer();

    CubeDepthBuffer(CubeDepthBuffer&& other);
    CubeDepthBuffer& operator=(CubeDepthBuffer&& other);

    unsigned int getTexture() const;

private:
    unsigned int _cubeDepthTexture = -1;

    CubeDepthBuffer(const CubeDepthBuffer& other) = delete;
    CubeDepthBuffer& operator=(const CubeDepthBuffer&) = delete;
};

} // namespace SceneDrawing
} // namespace Renderer