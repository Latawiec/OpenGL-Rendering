#pragma once
#include <Common/FramebufferBase.hpp>

#include <cstddef>
#include <utility>
#include "always_fail.hpp"

namespace Render {

class GraphicBuffer : public Common::FramebufferBase {
public:

    enum Output {
        Position = 0,
        Albedo,
        Normals,
        Depth,
        EdgeInfo,
        SIZE
    };

    GraphicBuffer(unsigned int width, unsigned int height);

    unsigned int getTexture(const Output texture);

private:
    unsigned int outputTextures_[Output::SIZE];

    template<Output O>
    void setupTexture() {
        static_assert("Not implemented for given Output target.");
    }
};

template<>
void GraphicBuffer::setupTexture<GraphicBuffer::Output::Position>();

template<>
void GraphicBuffer::setupTexture<GraphicBuffer::Output::Albedo>();

template<>
void GraphicBuffer::setupTexture<GraphicBuffer::Output::Normals>();

template<>
void GraphicBuffer::setupTexture<GraphicBuffer::Output::Depth>();

template<>
void GraphicBuffer::setupTexture<GraphicBuffer::Output::EdgeInfo>();

} // namespace Render