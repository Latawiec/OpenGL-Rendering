#pragma once

#include "Node.hpp"
#include <memory>

namespace Render {

class MeshNode : public Node {
    std::unique_ptr<IMesh> _mesh = nullptr;

public:
    MeshNode() = default;
    ~MeshNode() override = default;

    void Visit(INodeVisitor& visitor, const glm::mat4& transform) const override;

    void SetMesh(std::unique_ptr<IMesh> mesh);
    const IMesh* GetMesh() const;
};

} // namespace Render