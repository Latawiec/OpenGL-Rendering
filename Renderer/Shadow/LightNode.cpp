#include "Shadow/LightNode.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Render {

LightNode::LightNode() : Node(), _shadowMapBuffer(1024, 1024) {
    float near_plane = 1.0f, far_plane = 10.5f;
    _lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
}

LightNode::~LightNode() {
}

void LightNode::Visit(INodeVisitor& visitor, const glm::mat4& transform) const {
    visitor.Accept(*this, transform);
}

const Common::DepthBuffer& LightNode::GetShadowMap() const {
    return _shadowMapBuffer;
}

glm::mat4 LightNode::GetLightProjection() const {
    return _lightProjection;
}

glm::mat4 LightNode::GetLightView() const {
    constexpr const glm::vec3 SceneMiddle {0, 0, 0};
    constexpr const glm::vec3 SceneUp {0, 1, 0};
    const glm::mat4 transform = GetTransform();
    const auto& translationColumn = transform[3];
    const glm::vec3 translation{ translationColumn.x, translationColumn.y, translationColumn.z };

    return glm::lookAt(translation, SceneMiddle, SceneUp);
}

} // namespace Render