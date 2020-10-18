#pragma once

#include <map>

#include "Texture.hpp"
#include "VertexData.hpp"
#include "IMesh.hpp"

namespace Render {
namespace Contour {

class Mesh : public MeshImpl<Mesh> {

    const VertexDataBase _vertexData;
    //std::map<Texture::Type, Texture> _textures;
public:
    
    Mesh(VertexDataBase&& vertexData)
    : _vertexData(std::move(vertexData))
    {}

    const VertexDataBase& getVertexData() const {
        return _vertexData;
    }

    // const std::map<Texture::Type, Texture> getTextures() const {
    //     return _textures;
    // }
};

} // namespace Contour
} // namespace Render