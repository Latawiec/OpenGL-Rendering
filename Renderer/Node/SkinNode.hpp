#pragma once

#include "Node.hpp"
#include <glm/glm.hpp>

namespace Render {

class SkinNode : public Node {
    glm::mat4 _inverseBindTransform;

public:
    SkinNode(glm::mat4 transform, glm::mat4 inverseBindTransform);
    ~SkinNode() override = default;

    void Visit(INodeVisitor& visitor, const glm::mat4& transform) const override;

    void SetInverseBindTransform(const glm::mat4& SetInverseBindTransform);
    const glm::mat4& GetInverseBindTransform() const;
};

} // namespace Render