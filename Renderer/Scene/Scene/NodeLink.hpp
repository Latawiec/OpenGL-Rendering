#pragma once

#include "IdGenerator.hpp"

#include <vector>

namespace Render {
namespace Common {

struct NodeLink {

    NodeLink();
    NodeLink(IdGenerator::Type node);

    IdGenerator::Type GetNode() const;

    void AddChild(NodeLink&& node);
    std::vector<NodeLink>& GetChildren();
    const std::vector<NodeLink>& GetChildren() const;

private:
    IdGenerator::Type _node;
    std::vector<NodeLink> _childLinks;
};

} // namespace Common
} // namespace Render