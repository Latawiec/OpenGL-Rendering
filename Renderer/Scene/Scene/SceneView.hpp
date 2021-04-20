#pragma once

#include "Base/Node.hpp"
#include "Base/Camera.hpp"

namespace Renderer {
namespace Scene {

struct SceneView {
    Base::Node::IdType nodeId = Base::Node::INVALID_ID;
    Base::Camera::IdType cameraId = Base::Camera::INVALID_ID;
};

} // namespace Common
} // namespace Render