#pragma once

#include "Common/Mesh.hpp"
#include "Node.hpp"

#include <memory>

namespace Render {

class MeshNode : public Node {
    std::unique_ptr<Common::Mesh> _mesh = nullptr;

public:
    MeshNode() = default;
    ~MeshNode() override = default;

    void Visit(INodeVisitor& visitor, const glm::mat4& transform) const override;

    void SetMesh(std::unique_ptr<Common::Mesh> mesh);
    const Common::Mesh* GetMesh() const;
};

} // namespace Render