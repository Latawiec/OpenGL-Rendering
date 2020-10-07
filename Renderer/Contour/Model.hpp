#pragma once

#include <map>

#include "Texture.hpp"
#include "VertexData.hpp"
#include "IModel.hpp"

namespace Render {
namespace Contour {

class Model : public ModelImpl<Model> {
    using VertexDataType = typename VertexDataBase<
        Vec3, // Position
        Vec3, // Normal
        Vec2  // TexCoords
    >;

    const VertexDataType _vertexData;
    std::map<Texture::Type, Texture> _textures;
public:
    
    Model(VertexDataType vertexData)
    : _vertexData(vertexData)
    {}

    const VertexDataType& getVertexData() const {
        return _vertexData;
    }

    const std::map<Texture::Type, Texture> getTextures() const {
        return _textures;
    }
};

} // namespace Contour
} // namespace Render