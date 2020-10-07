#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "IModel.hpp"

namespace Render {

class Node {
    glm::mat4 _transform{1};
    const IModel* _model = nullptr;
    std::vector<std::unique_ptr<Node>> _childNodes;

public:
    Node() = default;
    Node(glm::mat4 transform);
    
    void SetTransform(glm::mat4 transform);
    const glm::mat4& GetTransform() const;

    void SetModel(const IModel* model);
    const IModel* GetModel() const;

    const std::vector<std::unique_ptr<Node>>& GetChildNodes() const;
    std::vector<std::unique_ptr<Node>>& GetChildNodes();
    void AddChildNode(std::unique_ptr<Node> node);
};

} // namespace Render