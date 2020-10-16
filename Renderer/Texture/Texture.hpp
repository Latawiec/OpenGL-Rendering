#pragma once 

#include <glad/glad.h>
#include <string_view>
#include <stdexcept>

namespace Render {

class Texture {
    int _width, _height, _channelsCount;
    unsigned int _texture;
public:

    Texture(std::string_view filepath);

    ~Texture();

    operator unsigned int() const {
        return _texture;
    }

    int width() const { return _width; }
    int height() const { return _height; }
    int channels() const { return _channelsCount; }

    enum class Type : uint8_t {
        Diffuse = 0,
        Specular,
        Normal,
        SIZE
    };

};

} // namespace Render