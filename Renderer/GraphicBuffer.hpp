#pragma once
#include "FramebufferBase.hpp"

#include <cstddef>
#include <utility>

namespace Render {

class GraphicBuffer : public FramebufferBase {
public:

    enum Output {
        Position = 0,
        Albedo,
        Normals,
        Depth,
        SIZE
    };

    GraphicBuffer(unsigned int width, unsigned int height) : FramebufferBase(width, height) {
        ScopedBinding bind(*this);
        glGenTextures(Output::SIZE, outputTextures_);

        // Setup all textures
        [this]<std::size_t ... I>(std::index_sequence<I...>){
            (setupTexture<static_cast<Output>(I)>(), ...);
        }(std::make_index_sequence<Output::SIZE>{});

        // Set attachments for all but Depth. I'll do it manually for now...
        const GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(sizeof(bufs)/sizeof(decltype(bufs[0])), bufs);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int getTexture(const Output texture) {
        return outputTextures_[texture];
    }

private:
    unsigned int outputTextures_[Output::SIZE];

    template<Output O>
    void setupTexture() {
        static_assert(Utils::always_fail<O>::value, "Not implemented for given Output target.");
    }

    template<>
    void setupTexture<Output::Position>() {
        const auto outputTexture = outputTextures_[Output::Position];
        glBindTexture(GL_TEXTURE_2D, outputTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);
    }

    template<>
    void setupTexture<Output::Albedo>() {
        const auto outputTexture = outputTextures_[Output::Albedo];
        glBindTexture(GL_TEXTURE_2D, outputTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, outputTexture, 0);
    }

    template<>
    void setupTexture<Output::Normals>() {
        const auto outputTexture = outputTextures_[Output::Normals];
        glBindTexture(GL_TEXTURE_2D, outputTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8_SNORM, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, outputTexture, 0);
    }

    template<>
    void setupTexture<Output::Depth>() {
        const auto outputTexture = outputTextures_[Output::Depth];
        glBindTexture(GL_TEXTURE_2D, outputTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, outputTexture, 0);
    }

};


} // namespace Render