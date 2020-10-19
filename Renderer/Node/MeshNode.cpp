#include "MeshNode.hpp"

namespace Render {

void MeshNode::Visit(INodeVisitor& visitor, const glm::mat4& transform) const {
    return visitor.Accept(*this, transform);
}

void MeshNode::SetMesh(std::unique_ptr<IMesh> mesh) {
    _mesh = std::move(mesh);
}

const IMesh* MeshNode::GetMesh() const {
    return _mesh.get();
}

}