#include "../UI/Components/CameraController.hpp"

namespace Renderer {
namespace UI {
namespace Components {

void CameraController::Draw() const
{
    if (_scene == nullptr) return;

    ImGui::Begin("Camera Controller");
    if (ImGui::BeginCombo("Cameras", (_selectedSceneView.cameraId == Scene::Base::Camera::INVALID_ID) ? "" : std::to_string(_selectedSceneView.cameraId).c_str())) {
        for (const auto& sceneView : _scene->GetSceneViews()) {
            if (ImGui::Selectable(std::to_string(sceneView.cameraId).c_str(), _selectedSceneView.cameraId == sceneView.cameraId)) {
                _selectedSceneView = sceneView;
            }
        }
        ImGui::EndCombo();
    }
    if (_selectedSceneView.cameraId != Scene::Base::Camera::INVALID_ID) {
        ImGui::Checkbox("Possessed", &_cameraPossessed);
    }
    ImGui::End();
};

void CameraController::SetScene(Scene::Scene* scenePtr)
{
    _scene = scenePtr;
}

const Scene::SceneView& CameraController::GetSelectedSceneView() const
{
    return _selectedSceneView;
}

bool CameraController::IsCameraPossessed() const
{
    return _cameraPossessed;
}

}; // Components 
}; // UI
}; // Renderer