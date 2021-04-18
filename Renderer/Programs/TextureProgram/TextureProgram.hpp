#pragma once 

#include "Scene/Base/VertexData.hpp"
#include "ShaderProgram.hpp"

#include "glm/glm.hpp"

namespace Renderer {
namespace Programs {

class TextureProgram {
    constexpr static auto processedTextureName = std::string_view("imageTexture");

    Scene::Base::VertexDataBase _vertexData;
    Base::ShaderProgram _shaderProgram;

public:
    TextureProgram();
    void draw(unsigned int texture) const;

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