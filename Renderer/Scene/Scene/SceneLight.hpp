#pragma once

#include "Base/Node.hpp"
#include "Base/DirectionalLight.hpp"

namespace Renderer {
namespace Scene {

struct SceneLight {
    Base::Node::IdType nodeId = Base::Node::INVALID_ID;
    Base::DirectionalLight::IdType directionalLightId = Base::DirectionalLight::INVALID_ID;
    Base::PointLight::IdType pointLightId = Base::PointLight::INVALID_ID;
    // Probably I'll just add more light ids here.
};

} // namespace Common
} // namespace Render