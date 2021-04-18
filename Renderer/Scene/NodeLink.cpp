#include "Scene/NodeLink.hpp"

namespace Render {
namespace Common {

NodeLink::NodeLink()
: _node(Node::INVALID_ID)
{}

NodeLink::NodeLink(Node::IdType node)
: _node(node)
{}

Node::IdType NodeLink::GetNode() const {
    return _node;
}

void NodeLink::AddChild(NodeLink&& node) {
    _childLinks.push_back(std::move(node));
}

std::vector<NodeLink>& NodeLink::GetChildren() {
    return _childLinks;
}

const std::vector<NodeLink>& NodeLink::GetChildren() const {
    return _childLinks;
}

} // namespace Common
} // namespace Render