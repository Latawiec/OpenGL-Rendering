#pragma once

#include "Base/Node.hpp"
#include "Base/Mesh.hpp"

namespace Renderer {
namespace Scene {

struct SceneObject {
    Base::Node::IdType nodeId = Base::Node::INVALID_ID;
    Base::Mesh::IdType meshId = Base::Mesh::INVALID_ID;
};

} // namespace Common
} // namespace Render