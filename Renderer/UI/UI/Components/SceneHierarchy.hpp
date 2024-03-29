#pragma once

#include "ComponentBase.hpp"

#include "imgui.h"
#include <Scene/Scene.hpp>

namespace Renderer {
namespace UI {
namespace Components {

struct SceneHierarchy : public ComponentBase {

    void Draw() const override;
    void SetScene(Scene::Scene* scenePtr);
    const Scene::SceneObject* GetSelected() const;

private:
    Scene::Scene* _scene = nullptr;
    
    mutable const Scene::SceneObject * _selectedItem = nullptr;
};

}; // Components 
}; // UI
}; // Renderer