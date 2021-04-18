#pragma once 

#include <glad/glad.h>
#include <string_view>
#include <stdexcept>

namespace Renderer {
namespace Scene {
namespace Base {

class Texture {
    int _width, _height, _channelsCount;
    unsigned int _texture;
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

    struct Params {
        Wrap wrapS = Wrap::Clamp;
        Wrap wrapT = Wrap::Clamp;
        FilterMag filterMag = FilterMag::Linear;
        FilterMin filtterMin = FilterMin::Linear;
    };

    Texture(std::string_view filepath, Texture::Params parameters = {});
    Texture(const int width, const int height, const int channelCount, const uint8_t* data, Texture::Params parameters = {});
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
    int channels() const { return _channelsCount; }

};

} // namespace Base
} // namespace Scene
} // namespace Renderer