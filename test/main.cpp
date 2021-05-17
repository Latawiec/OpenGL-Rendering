
// IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// IMGUI

#include <iostream>
#include <chrono>
#include <numeric>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Importer.hpp"
#include <Scene/Base/Mesh.hpp>
#include <Scene/Base/Texture.hpp>
#include <Scene/Scene.hpp>
#include <SceneDrawing/SceneDrawingManager.hpp>

#include <UI/Components/TransformEditor.hpp>
#include <UI/Components/SceneHierarchy.hpp>

int main() {

    constexpr int windowWidth = 1600;
    constexpr int windowHeight= 1200;
    constexpr float FPS = 15.0;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    Renderer::Scene::Scene mainScene;
    Renderer::Importer::Importer gltfImporter;
    auto imported = gltfImporter.importGltf(ASSETS_DIR "/scene_test.gltf", mainScene);
#ifndef NDEBUG
    std::cout << "SCENE!\n" << std::endl;
    std::cout << mainScene << std::endl;
#endif

    auto& cameraNode = mainScene.GetNode(mainScene.GetSceneViews().begin()->nodeId);
    const auto cacheTransform = cameraNode.GetTransform();

    auto& lightNode = mainScene.GetNode(mainScene.GetSceneLights().begin()->nodeId);
    const auto cacheLightTransform = lightNode.GetTransform();

    Renderer::SceneDrawing::SceneDrawingManager sceneDrawingManager(mainScene, windowWidth, windowHeight);

    // Just stick it in there...
    struct WindowData {
        Renderer::SceneDrawing::SceneDrawingManager* drawingManager;
    } windowData { &sceneDrawingManager };

    glfwSetWindowUserPointer(window, &windowData);    
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        WindowData* userData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        assert(userData != nullptr);

        userData->drawingManager->SetWindowSize(width, height);
    });

    sceneDrawingManager.SetResolution(windowWidth / 2, windowHeight / 2);

    const std::chrono::duration<float> frameTimeCap(1.f / FPS);

    // IMGUI
    // Setup Dear ImGui context
    gladLoadGL();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    const char* glsl_version = "#version 410";
    ImGui_ImplOpenGL3_Init(glsl_version);

    Renderer::UI::Components::TransformEditor transformEditor;
    Renderer::UI::Components::SceneHierarchy sceneHierarchy;
    sceneHierarchy.SetScene(&mainScene);
    // IMGUI

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const std::chrono::time_point<std::chrono::system_clock> startFrame = std::chrono::system_clock::now();
        sceneDrawingManager.Draw();

        const auto transform = cacheLightTransform * glm::rotate(glm::mat4{1}, glm::radians(90.f), glm::vec3(0, 1, 0));
        const auto transform2 = glm::rotate(cacheLightTransform, float(glfwGetTime())/1.f, glm::vec3(0, 1, 0));
        //lightNode.SetTransform(transform2);

        const static glm::mat4 cameraCachedTransform = [&]() {
            // just once.
            return sceneDrawingManager.GetNodeWorldTransform(mainScene.GetSceneViews().begin()->nodeId);
        }();

        //cameraNode.SetTransform(glm::rotate(cacheTransform, float(glm::radians(3.f * glfwGetTime())), glm::vec3(0, 1, 0)));

        // IMGUI
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        transformEditor.Append();
        sceneHierarchy.Append();
        bool showDemoWindow = true;
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // IMGUI

        glfwSwapBuffers(window);

        #ifndef NDEBUG
        while (auto error = glGetError()) {
            std::cerr << "Error: " << error << '\n';
        }
        #endif
        const std::chrono::time_point<std::chrono::system_clock> endFrame = std::chrono::system_clock::now();
        const auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(endFrame - startFrame);
        const auto frameTimeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(frameTimeCap - frameTime);

        std::this_thread::sleep_for(frameTimeLeft);

        transformEditor.Apply();
        transformEditor.SetNode(sceneHierarchy.GetSelected() ? &mainScene.GetNode(sceneHierarchy.GetSelected()->nodeId) : nullptr );
    }
	glfwTerminate();
	return 0;
}