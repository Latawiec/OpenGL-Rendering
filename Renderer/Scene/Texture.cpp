#include "Common/Texture.hpp"
#include "stb_image_proxy.hpp"


namespace Render {
namespace Common {

Texture::Texture(std::string_view filepath, Texture::Params parameters) {
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(parameters.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(parameters.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(parameters.filtterMin));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(parameters.filterMag));
    
    unsigned char* data = stbi_load(filepath.data(), &_width, &_height, &_channelsCount, 0);
    GLenum format;
    if (_channelsCount == 1) {
        format = GL_RED;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    }
    else if (_channelsCount == 3) {
        format = GL_RGB;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    }
    else if (_channelsCount == 4) {
        format = GL_RGBA;
    } else {
        throw std::runtime_error("Attempt to load image with number of channels that we cannot process.");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const int width, const int height, const int channelCount, const uint8_t* data, Texture::Params parameters)
    : _width(width)
    , _height(height)
    , _channelsCount(channelCount)
{
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(parameters.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(parameters.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(parameters.filtterMin));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(parameters.filterMag));
    
    GLenum format;
    if (_channelsCount == 1) {
        format = GL_RED;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    }
    else if (_channelsCount == 3) {
        format = GL_RGB;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    }
    else if (_channelsCount == 4) {
        format = GL_RGBA;
    } else {
        throw std::runtime_error("Attempt to load image with number of channels that we cannot process.");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture&& other) {
    const auto hold = _texture;
    this->_texture = other._texture;
    this->_width = other._width;
    this->_height = other._height;
    this->_channelsCount = other._channelsCount;

    other._texture = hold;
}

Texture& Texture::operator=(Texture&& other) {
    const auto hold = _texture;
    this->_texture = other._texture;
    this->_width = other._width;
    this->_height = other._height;
    this->_channelsCount = other._channelsCount;

    other._texture = hold;
    return *this;
}

Texture::~Texture() {
    if (_texture != -1) {
        glDeleteTextures(1, &_texture);
    }
}


} // namespace Common
} // namespace Render