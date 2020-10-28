#pragma once

#include "VertexData.hpp"

namespace Render {
namespace Common {

class Mesh {
    const VertexDataBase _vertexData;
public:
    Mesh(VertexDataBase&& vertexData);
    const VertexDataBase& getVertexData() const;
};

} // namespace Common
} // namespace Render