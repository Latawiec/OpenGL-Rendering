#pragma once

#include <map>

#include "Texture.hpp"
#include "VertexData.hpp"

namespace Render {
namespace Contour {

class Model {
    const VertexDataBase _vertexData;
    std::map<Texture::Type, Texture> _textures;
public:
    
    Model(VertexDataBase vertexData)
    : _vertexData(vertexData)
    {}

    const VertexDataBase& getVertexData() const {
        return _vertexData;
    }

    const std::map<Texture::Type, Texture> getTextures() const {
        return _textures;
    }
};

} // namespace Contour
} // namespace Render