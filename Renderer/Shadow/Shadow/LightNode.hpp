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
    glm::mat4 _lightView;

public:
    LightNode();
    virtual ~LightNode();

    void Visit(INodeVisitor& visitor, const glm::mat4& transform) const override;

    const glm::mat4& GetLightProjection() const;

    const glm::mat4& GetLightView() const;
};

} // namespace Render