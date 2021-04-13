#pragma once

#include <glm/glm.hpp>
#include <array>

namespace Render {
namespace MappingProgram {

using PropertiesSet = uint64_t;

enum class Properties : uint64_t {
    SKIN = 1 << 0,
    SHADOW_MAPPED = 1 << 1,
    BASE_COLOUR_TEXTURE = 1 << 2,
    NORMAL_MAP_TEXTURE = 1 << 3
};

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

static constexpr int MaxJointsCount = 32;
using JointsArray = std::array<glm::mat4, MaxJointsCount>;

struct Material {

    using IdType = uint64_t;

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

    private:
    std::array<Texture::IdType, size_t(ETexture::SIZE)> _textures = {};
    EDithering _dithering = EDithering::NONE;

};

} // namespace MappingProgram
} // namespace Render 
