#pragma once

#include "Base/Node.hpp"

#include <vector>

namespace Renderer {
namespace Scene {

struct NodeLink {

    NodeLink();
    NodeLink(Base::Node::IdType node);

    Base::Node::IdType GetNode() const;

    void AddChild(NodeLink&& node);
    std::vector<NodeLink>& GetChildren();
    const std::vector<NodeLink>& GetChildren() const;

private:
    Base::Node::IdType _node;
    std::vector<NodeLink> _childLinks;
};

} // namespace Scene
} // namespace Renderer