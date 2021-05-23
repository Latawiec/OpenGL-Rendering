#pragma once

#include "../FramebufferBase.hpp"

#include <cstddef>
#include <utility>

namespace Renderer {
namespace SceneDrawing {
namespace ContourPass {

class ContourPassBuffer : public Renderer::SceneDrawing::FramebufferBase {
public:

    enum Output {
        ContourMap = 0,
        SIZE
    };

    ContourPassBuffer(unsigned int width, unsigned int height);

    unsigned int getTexture(const Output texture);

private:
    unsigned int outputTextures_[Output::SIZE];

    template<Output O>
    void setupTexture() {
        static_assert("Not implemented for given Output target.");
    }
};

template<>
void ContourPassBuffer::setupTexture<ContourPassBuffer::Output::ContourMap>();

} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer