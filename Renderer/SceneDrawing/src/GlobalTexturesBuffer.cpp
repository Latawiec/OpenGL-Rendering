#include "../SceneDrawing/GlobalTexturesBuffer.hpp"

#include <glad/glad.h>

#include <array>

#ifndef ASSETS_TEXTURES_BLUE_NOISE_TEXTURE_PATH
#define ASSETS_TEXTURES_BLUE_NOISE_TEXTURE_PATH "Invalid texture path."
#endif

#ifndef ASSETS_TEXTURES_WHITE_NOISE_TEXTURE_PATH
#define ASSETS_TEXTURES_WHITE_NOISE_TEXTURE_PATH "Invalid texture path."
#endif

namespace Renderer {
namespace SceneDrawing {

GlobalTexturesBuffer::GlobalTexturesBuffer()
{
    // Setup all textures
    [this]<std::size_t ... I>(std::index_sequence<I...>){
        (setupTexture<static_cast<TextureName>(I)>(), ...);
    }(std::make_index_sequence<TextureName::SIZE>{});
}

const Scene::Base::Texture& GlobalTexturesBuffer::getTexture(const GlobalTexturesBuffer::TextureName texture) {
    return outputTextures_[texture];
}

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::Bayer4x4>()
{
    const unsigned int textureWidth = 4, textureHeight = 4;
    const std::array<float, 16> Bayer = {
        0.f/16.f,  8.f/16.f,  2.f/16.f,  10.f/16.f,
        12.f/16.f, 4.f/16.f,  14.f/16.f, 6.f/16.f,
        3.f/16.f,  11.f/16.f, 1.f/16.f,  9.f/16.f,
        15.f/16.f, 7.f/16.f,  13.f/16.f, 5.f/16.f
    };

    Scene::Base::Texture::Params parameters;
    parameters.wrapS = Scene::Base::Texture::Wrap::Repeat;
    parameters.wrapT = Scene::Base::Texture::Wrap::Repeat;
    parameters.filterMag = Scene::Base::Texture::FilterMag::Nearest;
    parameters.filterMin = Scene::Base::Texture::FilterMin::Nearest;
    parameters.internalFormat = Scene::Base::Texture::InternalFormat::R32F;
    parameters.format = Scene::Base::Texture::Format::R;
    parameters.type = Scene::Base::Texture::Type::Float;
    parameters.width = textureWidth;
    parameters.height = textureHeight;

    outputTextures_[TextureName::Bayer4x4] = Scene::Base::Texture(reinterpret_cast<const uint8_t*>(Bayer.data()), parameters);
}

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::Bayer8x8>()
{
    const unsigned int textureWidth = 8, textureHeight = 8;
    const std::array<float, 64> Bayer = {
        0.f/64.f,  32.f/64.f, 8.f/64.f,  40.f/64.f, 2.f/64.f,  34.f/64.f, 10.f/64.f, 42.f/64.f,
        38.f/64.f, 16.f/64.f, 56.f/64.f, 24.f/64.f, 50.f/64.f, 18.f/64.f, 58.f/64.f, 26.f/64.f,
        12.f/64.f, 44.f/64.f, 4.f/64.f,  36.f/64.f, 14.f/64.f, 46.f/64.f, 6.f/64.f,  38.f/64.f,
        60.f/64.f, 28.f/64.f, 52.f/64.f, 20.f/64.f, 62.f/64.f, 30.f/64.f, 54.f/64.f, 22.f/64.f,
        3.f/64.f,  35.f/64.f, 11.f/64.f, 43.f/64.f, 1.f/64.f,  33.f/64.f, 9.f/64.f,  41.f/64.f,
        51.f/64.f, 19.f/64.f, 59.f/64.f, 27.f/64.f, 49.f/64.f, 17.f/64.f, 57.f/64.f, 25.f/64.f,
        15.f/64.f, 47.f/64.f, 7.f/64.f,  39.f/64.f, 13.f/64.f, 45.f/64.f, 5.f/64.f,  37.f/64.f,
        63.f/64.f, 31.f/64.f, 55.f/64.f, 23.f/64.f, 61.f/64.f, 29.f/64.f, 53.f/64.f, 21.f/64.f
    };

    Scene::Base::Texture::Params parameters;
    parameters.wrapS = Scene::Base::Texture::Wrap::Repeat;
    parameters.wrapT = Scene::Base::Texture::Wrap::Repeat;
    parameters.filterMag = Scene::Base::Texture::FilterMag::Nearest;
    parameters.filterMin = Scene::Base::Texture::FilterMin::Nearest;
    parameters.internalFormat = Scene::Base::Texture::InternalFormat::R32F;
    parameters.format = Scene::Base::Texture::Format::R;
    parameters.type = Scene::Base::Texture::Type::Float;
    parameters.width = textureWidth;
    parameters.height = textureHeight;

    outputTextures_[TextureName::Bayer8x8] = Scene::Base::Texture(reinterpret_cast<const uint8_t*>(Bayer.data()), parameters);
}

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::WhiteNoise>()
{
    Scene::Base::Texture::Params parameters;
    parameters.wrapS = Scene::Base::Texture::Wrap::Repeat;
    parameters.wrapT = Scene::Base::Texture::Wrap::Repeat;
    parameters.filterMag = Scene::Base::Texture::FilterMag::Nearest;
    parameters.filterMin = Scene::Base::Texture::FilterMin::Nearest;
    
    outputTextures_[TextureName::WhiteNoise] = Scene::Base::Texture(ASSETS_TEXTURES_WHITE_NOISE_TEXTURE_PATH, parameters);
}

template<>
void GlobalTexturesBuffer::setupTexture<GlobalTexturesBuffer::BlueNoise>()
{
    Scene::Base::Texture::Params parameters;
    parameters.wrapS = Scene::Base::Texture::Wrap::Repeat;
    parameters.wrapT = Scene::Base::Texture::Wrap::Repeat;
    parameters.filterMag = Scene::Base::Texture::FilterMag::Nearest;
    parameters.filterMin = Scene::Base::Texture::FilterMin::Nearest;

    outputTextures_[TextureName::BlueNoise] = Scene::Base::Texture(ASSETS_TEXTURES_BLUE_NOISE_TEXTURE_PATH, parameters);
}

} // namespace SceneDrawing
} // namespace Renderer

