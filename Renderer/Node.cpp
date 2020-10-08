#include "Node.hpp"

namespace Render {

Node::Node(glm::mat4 transform) : _transform(transform) {}

void Node::SetTransform(glm::mat4 transform) {
    _transform = transform;
}

const glm::mat4& Node::GetTransform() const {
    return _transform;
}

void Node::SetModel(const IModel* model) {
    _model = model;
}

const IModel* Node::GetModel() const {
    return _model;
}

const std::vector<std::unique_ptr<Node>>& Node::GetChildNodes() const {
    return _childNodes;
}

std::vector<std::unique_ptr<Node>>& Node::GetChildNodes() {
    return _childNodes;
}

void Node::AddChildNode(std::unique_ptr<Node> node) {
    _childNodes.push_back(std::move(node));
}

} // namespace Render