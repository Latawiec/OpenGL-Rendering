#include "../Scene/Base/Node.hpp"

namespace Renderer {
namespace Scene {
namespace Base {

Node::Node()
: _transform(1) {}

Node::Node(const glm::mat4& transform)
: _transform(transform) {}

void Base::Node::SetTransform(const glm::mat4& transform) {
    _transform = transform;    
}

const glm::mat4& Base::Node::GetTransform() const {
    return _transform;
}

} // namespace Base
} // namespace Scene
} // namespace Renderer