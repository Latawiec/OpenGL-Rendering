#include "../Scene/Base/Mesh.hpp"

namespace Renderer {
namespace Scene {
namespace Base {
    
Mesh::Mesh(VertexDataBase&& vertexData)
: _vertexData(std::move(vertexData)) {}

const VertexDataBase& Mesh::getVertexData() const {
    return _vertexData;
}

} // namespace Base
} // namespace Scene
} // namespace Renderer