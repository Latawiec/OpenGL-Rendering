#include "../Scene/NodeLink.hpp"

namespace Renderer {
namespace Scene {

NodeLink::NodeLink()
: _node(Base::Node::INVALID_ID)
{}

NodeLink::NodeLink(Base::Node::IdType node)
: _node(node)
{}

Base::Node::IdType NodeLink::GetNode() const {
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

} // namespace Scene
} // namespace Renderer