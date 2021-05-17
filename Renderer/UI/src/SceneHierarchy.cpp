#include "UI/Components/SceneHierarchy.hpp"

namespace Renderer {
namespace UI {
namespace Components {

void SceneHierarchy::Append() const {
    ImGui::Begin("Scene Hierarchy");

    //ImGui::ListBoxHeader("", {300, 300});
    for (const auto& sceneObject : _scene->GetSceneObjects()) {
        bool isSelected = false;
        #ifndef NDEBUG
        const auto& node = _scene->GetNode(sceneObject.nodeId);
        isSelected = ImGui::Selectable(node.GetName().data(), _selectedItem == &sceneObject);
        #else
        isSelected = ImGui::Selectable(std::to_string(sceneObject.nodeId));
        #endif
        if (isSelected) {
            _selectedItem = &sceneObject;
        }
    }
    //ImGui::ListBoxFooter();
    ImGui::End();
}

void SceneHierarchy::SetScene(Scene::Scene* scenePtr) {
    _scene = scenePtr;
}

const Scene::SceneObject* SceneHierarchy::GetSelected() const {
    return _selectedItem;
}

}
}
}