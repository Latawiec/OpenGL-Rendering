#include "CameraNode.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render {
    
CameraNode::CameraNode() : Node() {}
CameraNode::CameraNode(const float fov, const float aspect, const Type cameraType) : Node(), _fov(fov), _aspectRatio(aspect), _type(cameraType) {} 

void CameraNode::Visit(INodeVisitor& visitor, const glm::mat4& transform) const {
    return visitor.Accept(*this, transform);
}

void CameraNode::SetFov(const float fov) {
    _fov = fov;
}

const float CameraNode::GetFov() const {
    return _fov;
}

void CameraNode::SetAspect(const float aspect) {
    _aspectRatio = aspect;
}

const float CameraNode::GetAspect() const {
    return _aspectRatio;
}

glm::mat4 CameraNode::GetProjectionTransform() const {
    // Don't care about ortographic for now.
    // Also, I should be able to only generate this matrix if FoV, Aspect Ratio or type changes,
    // instead of every single time this matrix is queried...
    return glm::perspective(_fov, _aspectRatio, 0.1f, 100.f);
}


} // namespace Render