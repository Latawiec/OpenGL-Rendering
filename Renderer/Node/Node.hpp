#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Common/Mesh.hpp"
#include "INodeVisitor.hpp"

namespace Render {

class Node {
   
protected:
    glm::mat4 _transform;
    uint64_t _nodeId;
    std::vector<std::unique_ptr<Node>> _childNodes;

public:
    Node();
    Node(glm::mat4 transform);
    virtual ~Node() = default;
    
    uint64_t GetId() const;

    virtual void Visit(INodeVisitor& visitor, const glm::mat4& transform) const;

    void SetTransform(glm::mat4 transform);
    glm::mat4 GetTransform() const;

    const std::vector<std::unique_ptr<Node>>& GetChildNodes() const;
    std::vector<std::unique_ptr<Node>>& GetChildNodes();
    void AddChildNode(std::unique_ptr<Node> node);
};

} // namespace Render