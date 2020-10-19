#pragma once

#include "Node.hpp"
#include <memory>

namespace Render {

class CameraNode : public Node {
public:
    enum Type {
        Perspective,
        Ortographic
    };

private:
    float _fov = 45.f;
    float _aspectRatio = 800.f/600.f;
    const Type _type = Type::Perspective;

public:
    CameraNode();
    CameraNode(const float fov, const float aspect, const Type cameraType = Type::Perspective);
    virtual ~CameraNode() = default;

    void Visit(INodeVisitor& visitor, const glm::mat4& transform) const override;

    void SetFov(const float fov);
    const float GetFov() const;

    void SetAspect(const float aspect);
    const float GetAspect() const;

    glm::mat4 GetViewTransform() const;
    glm::mat4 GetProjectionTransform() const;
};

} // namespace Render