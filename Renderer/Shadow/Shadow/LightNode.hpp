#pragma once

#include "Node.hpp"
#include <Common/DepthBuffer.hpp>

#include <glm/glm.hpp>

#include <memory>

namespace Render {

class LightNode : public Node {

private:
    glm::mat4 _lightProjection;
    Common::DepthBuffer _shadowMapBuffer;

public:
    LightNode();
    virtual ~LightNode();

    void Visit(INodeVisitor& visitor, const glm::mat4& transform) const override;

    const Common::DepthBuffer& GetShadowMap() const;

    glm::mat4 GetLightProjection() const;

    glm::mat4 GetLightView() const;
};

} // namespace Render