#pragma once

#include "Texture.hpp"

#include <glm/glm.hpp>
#include <array>
#include <optional>

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

    enum class ESolidColor : uint8_t {
        Albedo,
        Emissive,
        MetallicRoughness, // Metallic -> Blue; Rougness -> Green
        SIZE
    };

    Material() 
    {
        _textures.fill(Texture::INVALID_ID);
        _solidColors.fill(glm::vec4(0.f));
    }

    template<ETexture TextureType>
    bool hasTexture() const {
        return _textures[size_t(TextureType)] != Texture::INVALID_ID;
    }

    template<ETexture TextureType>
    const Texture::IdType getTexture() const {
        return _textures[size_t(TextureType)];
    }

    template<ESolidColor ColorType>
    glm::vec4 getSolidColor() const {
        return _solidColors[size_t(ColorType)];
    }

    template<ETexture TextureType>
    void setTexture(Texture::IdType texture) {
        _textures[size_t(TextureType)] = texture;
    }

    template<ESolidColor ColorType>
    void setSolidColor(glm::vec4 color) {
        _solidColors[size_t(ColorType)] = color;
    }

    EDithering getDithering() const { return _dithering; }
    void setDithering(EDithering value) { _dithering = value; }

    bool isCastingShadow() const { return _isCastingShadow; }
    void setCastingShadow(bool isCastingShadow) { _isCastingShadow = isCastingShadow; }

private:
    std::array<Texture::IdType, size_t(ETexture::SIZE)> _textures = {};
    std::array<glm::vec4, size_t(ESolidColor::SIZE)> _solidColors = {};
    EDithering _dithering = EDithering::NONE;
    bool _isCastingShadow = true;
};

} // namespace Base
} // namespace Scene
} // namespace Renderer
