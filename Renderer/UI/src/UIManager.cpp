#include "../UI/Manager/UIManager.hpp"

namespace Renderer {
namespace UI {
namespace Manager {

UIManager::UIManager(GLFWwindow* window, std::string_view glslVersion)
: _window(window)
, _glslVersion(glslVersion) {}

UIManager::~UIManager() {

}

bool UIManager::Initialize() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(_glslVersion.c_str());

    // I don't know what can go wrong here yet so just yeet a true;
    return true;
}

void UIManager::AddComponent(std::shared_ptr<Components::ComponentBase> componentPtr) {
    _components.insert(std::move(componentPtr));
}

void UIManager::Render() const {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for(const auto& component : _components) {
        component->Draw();
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Manager
} // namespace UI
} // namespace Renderer