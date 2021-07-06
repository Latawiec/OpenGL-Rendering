#pragma once

#include "ComponentBase.hpp"

#include "imgui.h"
#include <Scene/Base/Node.hpp>

namespace Renderer {
namespace UI {
namespace Components {

struct TransformEditor : public ComponentBase {

    void Draw() const override;
    void Apply();
    void SetNode(Scene::Base::Node* nodePtr);

private:
    Scene::Base::Node* _node = nullptr;
    mutable float _translate[3] {0, 0, 0};
    mutable float _scale[3] {1, 1, 1};
    mutable float _rotate[3] {0, 0, 0};
};

}; // Components 
}; // UI
}; // Renderer