#pragma once

#include "IdGenerator.hpp"

#include <vector>

#include <glm/glm.hpp>

namespace Render {
namespace Common {

struct NodeLink {

    enum Properties : uint16_t {
        MESH         = 1 << 0,
        CASTS_SHADOW = 1 << 1,
        SKIN         = 1 << 2,
        CAMERA       = 1 << 3
    };

    NodeLink();

    NodeLink(IdGenerator::Type node, uint16_t properties);

    IdGenerator::Type GetNode() const;

    void AddChild(NodeLink&& node);
    const std::vector<NodeLink>& GetChildren() const;

    void SetMesh(IdGenerator::Type mesh);
    IdGenerator::Type GetMesh() const;

    void SetCamera(IdGenerator::Type camera);
    IdGenerator::Type GetCamera() const;

    void SetSkin(IdGenerator::Type skin);
    IdGenerator::Type GetSkin() const;

    void CacheTransform(const glm::mat4& transform);
    const glm::mat4& GetCachedTransform() const;

private:
    glm::mat4 _cachedTransform;
    IdGenerator::Type _node;
    IdGenerator::Type _mesh;
    IdGenerator::Type _camera;
    IdGenerator::Type _skin;
    uint16_t _properties;
    std::vector<NodeLink> _childLinks;
};

} // namespace Common
} // namespace Render