#include "Scene/Node.hpp"

namespace Render {
namespace Common {

Node::Node()
: _transform(1) {}

Node::Node(const glm::mat4& transform)
: _transform(transform) {}

void Node::SetTransform(const glm::mat4& transform) {
    _transform = transform;    
}

const glm::mat4& Node::GetTransform() const {
    return _transform;
}

} // namespace Common
} // namespace Render