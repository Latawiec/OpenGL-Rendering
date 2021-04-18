#pragma once

#include "Node.hpp"

#include <vector>

namespace Render {
namespace Common {

struct NodeLink {

    NodeLink();
    NodeLink(Node::IdType node);

    Node::IdType GetNode() const;

    void AddChild(NodeLink&& node);
    std::vector<NodeLink>& GetChildren();
    const std::vector<NodeLink>& GetChildren() const;

private:
    Node::IdType _node;
    std::vector<NodeLink> _childLinks;
};

} // namespace Common
} // namespace Render