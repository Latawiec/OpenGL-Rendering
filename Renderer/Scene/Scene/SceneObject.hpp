#pragma once

#include "Base/Node.hpp"
#include "Base/Mesh.hpp"
#include "Base/Skin.hpp"
#include "Base/Material.hpp"

namespace Renderer {
namespace Scene {

struct SceneObject {
    Base::Node::IdType nodeId = Base::Node::INVALID_ID;
    Base::Mesh::IdType meshId = Base::Mesh::INVALID_ID;
    Base::Skin::IdType skinId = Base::Skin::INVALID_ID;
    Base::Material::IdType materialId = Base::Material::INVALID_ID;
};

} // namespace Common
} // namespace Render