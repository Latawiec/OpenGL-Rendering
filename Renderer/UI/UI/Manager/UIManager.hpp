#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Components/ComponentBase.hpp"

// IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// IMGUI

#include <string_view>
#include <memory>
#include <set>

namespace Renderer {
namespace UI {
namespace Manager {

struct UIManager {

    UIManager(GLFWwindow* window, std::string_view glslVersion);
    ~UIManager();

    bool Initialize();
    void AddComponent(std::shared_ptr<Components::ComponentBase> componentPtr);
    void Render() const;

private:
    GLFWwindow* _window;
    std::string _glslVersion;

    std::set<std::shared_ptr<Components::ComponentBase>> _components;
};

} // namespace Manager
} // namespace UI
} // namespace Renderer