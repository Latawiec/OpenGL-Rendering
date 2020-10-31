#pragma once

#include "Node.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace Render {

class LightNode : public Node {
public:
    enum Type {
        Directional
        // More?
    };

private:
    const Type _type = Type::Directional;
    glm::mat4 _lightProjection;

public:
    LightNode();
    virtual ~LightNode();

    void Visit(INodeVisitor& visitor, const glm::mat4& transform) const override;

    glm::mat4 GetLightProjection() const;

    glm::mat4 GetLightView() const;
};

} // namespace Render