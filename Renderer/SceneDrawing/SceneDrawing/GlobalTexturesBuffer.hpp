#pragma once

#include <Scene/Base/Texture.hpp>

#include <cstddef>
#include <utility>

namespace Renderer {
namespace SceneDrawing {

class GlobalTexturesBuffer {
public:

    enum TextureName {
        Bayer4x4,
        Bayer8x8,
        WhiteNoise,
        BlueNoise,
        SIZE
    };

    GlobalTexturesBuffer();

    const Scene::Base::Texture& getTexture(const TextureName texture);

private:
    Scene::Base::Texture outputTextures_[TextureName::SIZE];

    template<TextureName O>
    void setupTexture() {
        static_assert("Not implemented for given Output target.");
    }
};

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::Bayer4x4>();

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::Bayer8x8>();

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::WhiteNoise>();

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::BlueNoise>();

} // namespace SceneDrawing
} // namespace Renderer