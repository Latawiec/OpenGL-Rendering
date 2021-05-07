#include <iostream>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Importer.hpp"
#include <Scene/Base/Mesh.hpp>
#include <Scene/Base/Texture.hpp>
#include <Scene/Scene.hpp>
#include <SceneDrawing/SceneDrawingManager.hpp>

int main() {

    constexpr int windowWidth = 1600;
    constexpr int windowHeight= 1200;

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
    std::cout << "SCENE!\n" << std::endl;
    std::cout << mainScene << std::endl;

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

    while(!glfwWindowShouldClose(window)) {
        //stolenLight.SetTransform(glm::translate(glm::mat4(1), glm::vec3(5 * glm::sin(glfwGetTime()), 2.5, 1)));
        sceneDrawingManager.Draw();

        const auto transform = cacheLightTransform * glm::rotate(glm::mat4{1}, glm::radians(90.f), glm::vec3(0, 1, 0));
        const auto transform2 = glm::rotate(cacheTransform, float(glfwGetTime())/1.f, glm::vec3(0, 1, 0));
        lightNode.SetTransform(transform2);

        glfwSwapBuffers(window);
        glfwPollEvents();

        #ifndef NDEBUG
        while (auto error = glGetError()) {
            std::cerr << "Error: " << error << '\n';
        }
        #endif
    }
	glfwTerminate();
	return 0;
}