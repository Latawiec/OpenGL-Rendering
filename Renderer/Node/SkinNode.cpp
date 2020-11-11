#include "SkinNode.hpp"

namespace Render {

SkinNode::SkinNode(glm::mat4 transform, glm::mat4 inverseBindTransform)
: Node(transform)
, _inverseBindTransform(inverseBindTransform)
{}

void SkinNode::Visit(INodeVisitor& visitor, const glm::mat4& transform) const {
    return visitor.Accept(*this, transform);
}

void SkinNode::SetInverseBindTransform(const glm::mat4& inverseBindTransform) {
    _inverseBindTransform = inverseBindTransform;
}

const glm::mat4& SkinNode::GetInverseBindTransform() const {
    return _inverseBindTransform;
}

} // namespace Render