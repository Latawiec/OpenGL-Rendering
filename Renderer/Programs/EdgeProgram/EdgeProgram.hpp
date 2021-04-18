#pragma once

#include <string_view>

#include "Scene/Base/Texture.hpp"
#include "Scene/Base/VertexData.hpp"
#include "ShaderProgram.hpp"

namespace Renderer {
namespace Programs {

class EdgeProgram {

    Scene::Base::VertexDataBase _vertexData;
    Base::ShaderProgram _program;
    float _imageWidth = 1.f;
    float _imageHeight = 1.f;
    constexpr static std::string_view u_edgeMapTexture = "edgeMap_tex";
    constexpr static std::string_view u_imageSize = "imageSize";
    void prepareTextures(const unsigned int& texture) const;
    void prepareUniforms() const;

public:
    EdgeProgram();

    void Draw(const unsigned int& texture) const;

    void SetImageSize(const float& width, const float& height);

    EdgeProgram(EdgeProgram&) = delete;
    EdgeProgram(EdgeProgram&&) = delete;
    EdgeProgram& operator=(EdgeProgram&) = delete;
    EdgeProgram& operator=(EdgeProgram&&) = delete;

private:
    constexpr static inline glm::vec3 quadCoords[4] = {
        {-1.f, -1.f, 0.f},
        {1.f, -1.f, 0.f},
        {1.f, 1.f, 0.f},
        {-1.f, 1.f, 0.f}
    };

    constexpr static inline unsigned int indices[6] = {
        0, 1, 2, 2, 0, 3
    };
};

} // namespace Programs
} // namespace Renderer 