#include "Shadow/LightNode.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Render {

LightNode::LightNode() : Node() {
    float near_plane = 1.0f, far_plane = 7.5f;
    _lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
}

LightNode::~LightNode() {
}

void LightNode::Visit(INodeVisitor& visitor, const glm::mat4& transform) const {
    visitor.Accept(*this, transform);
}

const glm::mat4& LightNode::GetLightProjection() const {
    return _lightProjection;
}

const glm::mat4& LightNode::GetLightView() const {
    constexpr const glm::vec3 SceneMiddle {0, 0, 0};
    constexpr const glm::vec3 SceneUp {0, 1, 0};
    const glm::mat4 transform = GetTransform();
    const float x = transform[3][0];
    const float y = transform[3][1];
    const float z = transform[3][2];
    const glm::vec3 translation{x, y, z};

    return glm::lookAt(translation, SceneMiddle, SceneUp);
}

} // namespace Render