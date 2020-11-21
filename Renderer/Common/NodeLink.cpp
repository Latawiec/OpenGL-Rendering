#include "Common/NodeLink.hpp"

namespace Render {
namespace Common {

NodeLink::NodeLink()
: _node(IdGenerator::INVALID)
{}

NodeLink::NodeLink(IdGenerator::Type node)
: _node(node)
{}

IdGenerator::Type NodeLink::GetNode() const {
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