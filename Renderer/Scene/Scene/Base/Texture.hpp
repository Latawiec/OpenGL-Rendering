#pragma once 

#include <glad/glad.h>
#include <string_view>
#include <stdexcept>

namespace Renderer {
namespace Scene {
namespace Base {

class Texture {
    int _width, _height;
    unsigned int _texture = -1;
public:
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    enum Wrap : GLint {
        Clamp = GL_CLAMP_TO_EDGE,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        Repeat = GL_REPEAT
    };

    enum class FilterMag : GLint {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR
    };

    enum class FilterMin : GLint {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class InternalFormat : GLint {
        R = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        R32F = GL_R32F
        // I'll add more if I need to.
    };

    enum class Format : GLint {
        R = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA
        // I'll add more if I need to.
    };

    enum class Type : GLint {
        UnsignedInt = GL_UNSIGNED_INT,
        Int = GL_INT,
        UnsignedByte = GL_UNSIGNED_BYTE,
        Float = GL_FLOAT
        // I'll add more if I need to.
    };

    struct Params {
        Wrap wrapS = Wrap::Clamp;
        Wrap wrapT = Wrap::Clamp;
        FilterMag filterMag = FilterMag::Linear;
        FilterMin filterMin = FilterMin::Linear;
        InternalFormat internalFormat = InternalFormat::RGBA;
        Format format = Format::RGBA;
        Type type = Type::UnsignedByte;
        unsigned int width = 0, height = 0;
    };

    Texture(){};
    Texture(std::string_view filepath, Texture::Params parameters = {});
    Texture(const uint8_t* data, Texture::Params parameters = {});
    ~Texture();

    Texture(Texture&& other);
    Texture& operator=(Texture&&);

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    operator unsigned int() const {
        return _texture;
    }

    int width() const { return _width; }
    int height() const { return _height; }

};

} // namespace Base
} // namespace Scene
} // namespace Renderer