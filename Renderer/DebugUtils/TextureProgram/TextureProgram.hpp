#pragma once 

#include "glm/glm.hpp"

#include "VertexData.hpp"
#include "ShaderProgram.hpp"

namespace Render {
namespace DebugUtils {

class TextureProgram {
    constexpr static auto processedTextureName = std::string_view("imageTexture");

    VertexDataBase<Vec3> _vertexData;
    ShaderProgram _shaderProgram;

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

} // namespace DebugUtils
} // namespace Render