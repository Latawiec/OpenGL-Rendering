#pragma once

#include "Texture.hpp"

#include <glm/glm.hpp>
#include <array>

namespace Renderer {
namespace Scene {
namespace Base {

struct Material {
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;


    enum class EDithering : uint8_t {
        NONE = 0,
        Bayer4x4,
        Bayer8x8,
        Bayer16x16,
        Bayer32x32,
        Bayer64x64,
        WhiteNoise,
        BlueNoise
    };

    enum class ETexture : uint8_t {
        EdgeMap = 0,
        Albedo,
        Emissive,
        Normal,
        MetallicRoughness, // Metallic -> Blue; Rougness -> Green
        Occlusion,
        SIZE
    };

    template<ETexture TextureType>
    bool hasTexture() const {
        return _textures[size_t(TextureType)].has_value();
    }

    template<ETexture TextureType>
    const Texture::IdType getTexture() const {
        return _textures[size_t(TextureType)];
    }

    template<ETexture TextureType>
    void setTexture(Texture::IdType texture) {
        _textures[size_t(TextureType)] = texture;
    }

    EDithering getDithering() const { return _dithering; }
    void setDithering(EDithering value) { _dithering = value; }

    bool isCastingShadow() const { return _isCastingShadow; }
    void setCastingShadow(bool isCastingShadow) { _isCastingShadow = isCastingShadow; }

private:
    std::array<Texture::IdType, size_t(ETexture::SIZE)> _textures = {};
    EDithering _dithering = EDithering::NONE;
    bool _isCastingShadow = true;
};

} // namespace Base
} // namespace Scene
} // namespace Renderer
