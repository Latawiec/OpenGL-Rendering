#include "Texture.hpp"
#include "stb_image_proxy.hpp"


namespace Render {

Texture::Texture(std::string_view filepath) {
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
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

Texture::~Texture() {
    glDeleteTextures(1, &_texture);
}



} // namespace Render