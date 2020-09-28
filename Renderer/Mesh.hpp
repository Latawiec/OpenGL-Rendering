#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string_view>
#include <vector>
#include <map>
#include <string>
#include <array>

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "VertexData.hpp"

namespace Render {

enum class TextureType : uint8_t {
    Diffuse = 0,
    Specular,
    Normal,
    SIZE
};

class Mesh {
    VertexDataBase _vertexData;
public:
    
    Mesh(VertexDataBase vertexData)
    : _vertexData(vertexData)
    {}

    const VertexDataBase& getVertexData() const {
        return _vertexData;
    }

    void Draw(ShaderProgram& shader) const {

        VertexDataBase::ScopedBinding bind(_vertexData);
        glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
    }
};

} // namespace Render