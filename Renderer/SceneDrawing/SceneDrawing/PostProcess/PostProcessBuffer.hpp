#pragma once

#include "../FramebufferBase.hpp"

#include <cstddef>
#include <utility>

namespace Renderer {
namespace SceneDrawing {
namespace PostProcess {

class PostProcessBuffer : public Renderer::SceneDrawing::FramebufferBase {
public:

    enum Output {
        PostProcessOutput = 0,
        SIZE
    };

    PostProcessBuffer(unsigned int width, unsigned int height);

    unsigned int getTexture(const Output texture);

private:
    unsigned int outputTextures_[Output::SIZE];

    template<Output O>
    void setupTexture() {
        static_assert("Not implemented for given Output target.");
    }
};

template<>
void PostProcessBuffer::setupTexture<PostProcessBuffer::Output::PostProcessOutput>();

} // namespace PostProcess
} // namespace SceneDrawing
} // namespace Renderer