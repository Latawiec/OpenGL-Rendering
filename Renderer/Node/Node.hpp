#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "IMesh.hpp"

namespace Render {

class Node {
    glm::mat4 _transform{1};
    std::unique_ptr<IMesh> _mesh = nullptr;
    std::vector<std::unique_ptr<Node>> _childNodes;

public:
    Node() = default;
    Node(glm::mat4 transform);
    
    void SetTransform(glm::mat4 transform);
    const glm::mat4& GetTransform() const;

    void SetMesh(std::unique_ptr<IMesh> mesh);
    const IMesh* GetMesh() const;

    const std::vector<std::unique_ptr<Node>>& GetChildNodes() const;
    std::vector<std::unique_ptr<Node>>& GetChildNodes();
    void AddChildNode(std::unique_ptr<Node> node);
};

} // namespace Render