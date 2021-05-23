#pragma once

#include "../FramebufferBase.hpp"

#include <cstddef>
#include <utility>

namespace Renderer {
namespace SceneDrawing {
namespace ContourPass {

class CombinePassBuffer : public Renderer::SceneDrawing::FramebufferBase {
public:

    enum Output {
        SceneColor = 0,
        SIZE
    };

    CombinePassBuffer(unsigned int width, unsigned int height);

    unsigned int getTexture(const Output texture);

private:
    unsigned int outputTextures_[Output::SIZE];

    template<Output O>
    void setupTexture() {
        static_assert("Not implemented for given Output target.");
    }
};

template<>
void CombinePassBuffer::setupTexture<CombinePassBuffer::Output::SceneColor>();

} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer