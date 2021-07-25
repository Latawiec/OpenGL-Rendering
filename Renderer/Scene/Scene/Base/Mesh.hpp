#pragma once

#include "Base/Node.hpp"
#include "Base/Primitive.hpp"
#include "Base/Material.hpp"
#include "Base/Skin.hpp"

namespace Renderer {
namespace Scene {
namespace Base {

struct Mesh {
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    using MaterialPrimitivePair = std::pair<Base::Material::IdType, Base::Primitive::IdType>;

    std::vector<MaterialPrimitivePair> materialPrimitiveIdsPairs;
    Base::Skin::IdType skinId = Base::Skin::INVALID_ID;
};

} // namespace Base
} // namespace Common
} // namespace Render