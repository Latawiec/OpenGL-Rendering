#include "MeshNode.hpp"

namespace Render {

void MeshNode::Visit(INodeVisitor& visitor, const glm::mat4& transform) const {
    return visitor.Accept(*this, transform);
}

void MeshNode::SetMesh(std::unique_ptr<Common::Mesh> mesh) {
    _mesh = std::move(mesh);
}

const Common::Mesh* MeshNode::GetMesh() const {
    return _mesh.get();
}

}