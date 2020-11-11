#include "Common/Mesh.hpp"

namespace Render {
namespace Common {
    
Mesh::Mesh(VertexDataBase&& vertexData)
: _vertexData(std::move(vertexData)) {}

const VertexDataBase& Mesh::getVertexData() const {
    return _vertexData;
}

} // namespace Common
} // namespace Render