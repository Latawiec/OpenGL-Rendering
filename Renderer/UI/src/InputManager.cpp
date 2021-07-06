#include "../UI/Manager/InputManager.hpp"

// IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// IMGUI

namespace Renderer {
namespace UI {
namespace Manager {

InputManager::InputManager() {
}

InputManager::KeyboardListenerIdGenerator::Type InputManager::RegisterKeyboardListener(std::unique_ptr<InputManager::IKeyboardListener> pListener) {
    const auto id = _keyboardListenersIdGenerator.GenerateId();
    _keyboardListeners.emplace(id, std::move(pListener));
    return id;
}

bool InputManager::RemoveKeyboardListener(const InputManager::KeyboardListenerIdGenerator::Type& listenerId) {
    return _keyboardListeners.erase(listenerId) != 0;
}

void InputManager::HandleInput() const {
    auto& io = ImGui::GetIO();
    
    for (auto& keyboardListener : _keyboardListeners) {
        keyboardListener.second->OnHandleInput(io.KeysDown);
    }
}

} // namespace Manager
} // namespace UI
} // namespace Renderer