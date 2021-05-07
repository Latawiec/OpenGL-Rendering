#include "../Scene/Base/Texture.hpp"
#include "stb_image_proxy.hpp"


namespace Renderer {
namespace Scene {
namespace Base {

Texture::Texture(std::string_view filepath, Texture::Params parameters) {
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(parameters.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(parameters.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(parameters.filterMin));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(parameters.filterMag));
    
    int channelsCount;
    unsigned char* data = stbi_load(filepath.data(), &_width, &_height, &channelsCount, 0);
    GLenum format;
    if (channelsCount == 1) {
        format = GL_RED;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ZERO);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ZERO);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ZERO);
    }
    else if (channelsCount == 2) {
        format = GL_RG;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ZERO);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ZERO);
    }
    else if (channelsCount == 3) {
        format = GL_RGB;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ZERO);
    }
    else if (channelsCount == 4) {
        format = GL_RGBA;
    } else {
        throw std::runtime_error("Attempt to load image with number of channels that we cannot process.");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const uint8_t* data, Texture::Params parameters)
    : _width(parameters.width)
    , _height(parameters.height)
{
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(parameters.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(parameters.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(parameters.filterMin));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(parameters.filterMag));
    
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(parameters.internalFormat), _width, _height, 0, static_cast<GLint>(parameters.format), static_cast<GLint>(parameters.type), data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture&& other) {
    const auto hold = _texture;
    this->_texture = other._texture;
    this->_width = other._width;
    this->_height = other._height;

    other._texture = hold;
}

Texture& Texture::operator=(Texture&& other) {
    const auto hold = _texture;
    this->_texture = other._texture;
    this->_width = other._width;
    this->_height = other._height;

    other._texture = hold;
    return *this;
}

Texture::~Texture() {
    if (_texture != -1) {
        glDeleteTextures(1, &_texture);
    }
}

} // namespace Base
} // namespace Scene
} // namespace Renderer