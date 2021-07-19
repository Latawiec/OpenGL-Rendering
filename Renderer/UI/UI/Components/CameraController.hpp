#pragma once

#include "ComponentBase.hpp"

#include "imgui.h"
#include <Scene/Scene.hpp>
#include <Scene/Base/Camera.hpp>

namespace Renderer {
namespace UI {
namespace Components {

struct CameraController : public ComponentBase {

    void Draw() const override;
    void SetScene(Scene::Scene* scenePtr);

    const Scene::SceneView& GetSelectedSceneView() const;
    bool IsCameraPossessed() const;
    float GetCameraSpeed() const;

private:
    Scene::Scene* _scene = nullptr;
    
    mutable Scene::SceneView _selectedSceneView = {};
    mutable bool _cameraPossessed = false;
    mutable float _cameraSpeed = 1.0f;
};

}; // Components 
}; // UI
}; // Renderer