#include "../Scene/Base/Primitive.hpp"

namespace Renderer {
namespace Scene {
namespace Base {
    
Primitive::Primitive(VertexDataBase&& vertexData)
: _vertexData(std::move(vertexData)) {}

const VertexDataBase& Primitive::getVertexData() const {
    return _vertexData;
}

} // namespace Base
} // namespace Scene
} // namespace Renderer