#pragma once

#include <optional>

namespace Render {

struct Node;
struct CameraNode;
struct MeshNode;
struct LightNode;

struct INodeVisitor {
    virtual ~INodeVisitor() = default;
    virtual void Accept(const Node&, const glm::mat4& transform) {};
    virtual void Accept(const CameraNode&, const glm::mat4& transform) {};
    virtual void Accept(const MeshNode&, const glm::mat4& transform) {};
    virtual void Accept(const LightNode&, const glm::mat4& transform) {};
};

}