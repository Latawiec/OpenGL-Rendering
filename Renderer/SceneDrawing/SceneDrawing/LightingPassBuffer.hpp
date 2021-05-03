#pragma once

#include "../SceneDrawing/FramebufferBase.hpp"

#include <cstddef>
#include <utility>

namespace Renderer {
namespace SceneDrawing {

class LightingPassBuffer : public Renderer::SceneDrawing::FramebufferBase {
public:

    enum Output {
        Diffuse = 0,
        Specular,
        SIZE
    };

    LightingPassBuffer(unsigned int width, unsigned int height);

    unsigned int getTexture(const Output texture);

private:
    unsigned int outputTextures_[Output::SIZE];

    template<Output O>
    void setupTexture() {
        static_assert("Not implemented for given Output target.");
    }
};

template<>
void LightingPassBuffer::setupTexture<LightingPassBuffer::Output::Diffuse>();

template<>
void LightingPassBuffer::setupTexture<LightingPassBuffer::Output::Specular>();

} // namespace SceneDrawing
} // namespace Renderer