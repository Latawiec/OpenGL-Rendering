#pragma once

#include "../FramebufferBase.hpp"

#include <cstddef>
#include <utility>

namespace Renderer {
namespace SceneDrawing {
namespace BasePass {

class BasePassBuffer : public Renderer::SceneDrawing::FramebufferBase {
public:

    enum Output {
        Position = 0,
        Albedo,
        Normals,
        MetallicRoughness,
        SilhouetteMap,
        Dither,
        Depth,
        SIZE
    };

    BasePassBuffer(unsigned int width, unsigned int height);

    unsigned int getTexture(const Output texture);

private:
    unsigned int outputTextures_[Output::SIZE];

    template<Output O>
    void setupTexture() {
        static_assert("Not implemented for given Output target.");
    }
};

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Position>();

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Albedo>();

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Normals>();

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::MetallicRoughness>();

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::SilhouetteMap>();

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Dither>();

template<>
void BasePassBuffer::setupTexture<BasePassBuffer::Output::Depth>();

} // namespace BasePass
} // namespace SceneDrawing
} // namespace Renderer